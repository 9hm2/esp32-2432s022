// Serial Terminal (VT100) — Raspberry Pi soros konzol az ESP32-2432S022C-n
// ========================================================================
// Valódi VT100/ANSI terminál: karakterrács + kurzor + escape-szekvenciák
// (kurzormozgás, törlés, görgetés, SGR színek/attribútumok). A rácsot egyedi
// LVGL rajzolással jelenítjük meg, kis monospace fonttal.
//
// BEKÖTÉS a Raspberry Pi-hez (a panel P1 / "4P 1.25 Power supply base" portja,
// UART0): Pi TXD -> RXD(GPIO3), Pi RXD <- TXD(GPIO1), GND <-> GND. 3.3V logika.
// Az app NEM ír debug üzenetet az UART0-ra, hogy ne zavarja a kapcsolatot.

#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <esp_lcd_panel_ops.h>

#include "config.h"
#include "launcher_return.h"
#include "vt100.h"

static TermConfig cfg;
static Vt100 vt;

// Kis monospace font (DejaVu Sans Mono, 8px, szoros sor) — keskeny, sok fér ki.
// Cella: 5 px széles x 8 px magas (= a font line_height-ja a tömör igazításhoz).
extern const lv_font_t term_font;
static constexpr int CELL_W = 5;
static constexpr int CELL_H = 8;
static constexpr int TOP_H = 22; // felso sáv (állapot + gombok)

// --- UI elemek -------------------------------------------------------------
static lv_obj_t *term_obj = nullptr;   // egyedi rajzolású terminál
static lv_obj_t *status_lbl = nullptr;
static lv_obj_t *kb_overlay = nullptr;
static lv_obj_t *input_ta = nullptr;
static lv_obj_t *set_overlay = nullptr;
static lv_obj_t *dd_baud = nullptr;
static lv_obj_t *dd_le = nullptr;
static lv_obj_t *sw_echo = nullptr;
static lv_obj_t *dd_rot = nullptr;

static bool g_rebuild = false;

static int32_t scrW() { return lv_display_get_horizontal_resolution(NULL); }
static int32_t scrH() { return lv_display_get_vertical_resolution(NULL); }
static int32_t imin(int32_t a, int32_t b) { return a < b ? a : b; }

static void build_ui();
static void show_keys();

static void update_status()
{
    static const char *le_short[] = {"-", "LF", "CRLF", "CR"};
    if (vt.scroll() > 0)
        lv_label_set_text_fmt(status_lbl, LV_SYMBOL_UP " SCROLL -%d/%d  (drag)",
                              vt.scroll(), vt.scrollbackCount());
    else
        lv_label_set_text_fmt(status_lbl,
                              LV_SYMBOL_USB " RPi %lu 8N1 %s%s %dx%d",
                              (unsigned long)cfg.baud,
                              le_short[cfg.lineEnding <= 3 ? cfg.lineEnding : 0],
                              cfg.localEcho ? " echo" : "", vt.cols(), vt.rows());
}

// --- Terminál rajzolása (egyedi LVGL draw) ---------------------------------

static void term_draw_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target_obj(e);
    lv_layer_t *layer = lv_event_get_layer(e);
    lv_area_t area;
    lv_obj_get_content_coords(obj, &area);
    const int32_t ox = area.x1, oy = area.y1;

    lv_draw_rect_dsc_t rd;
    lv_draw_rect_dsc_init(&rd);
    rd.bg_opa = LV_OPA_COVER;

    lv_draw_letter_dsc_t ld;
    lv_draw_letter_dsc_init(&ld);
    ld.font = &term_font;
    ld.opa = LV_OPA_COVER;

    const bool live = (vt.scroll() == 0);
    for (int y = 0; y < vt.rows(); y++)
    {
        for (int x = 0; x < vt.cols(); x++)
        {
            const VtCell &c = vt.viewCell(x, y); // scrollback-figyelo
            lv_color_t fg = (c.flags & VT_FG_DEF) ? Vt100::defaultFg()
                                                  : Vt100::palette(c.fg);
            lv_color_t bg = (c.flags & VT_BG_DEF) ? Vt100::defaultBg()
                                                  : Vt100::palette(c.bg);
            bool inv = (c.flags & VT_INVERSE) != 0;
            if (live && vt.cursorVisible() && x == vt.curX() && y == vt.curY())
                inv = !inv; // kurzor csak élo nézetben

            if (inv)
            {
                lv_color_t t = fg;
                fg = bg;
                bg = t;
            }

            const int32_t cx = ox + x * CELL_W, cy = oy + y * CELL_H;

            // Háttér: csak ha nem az alap (fekete), vagy invertált/kurzor.
            if (inv || !(c.flags & VT_BG_DEF))
            {
                rd.bg_color = bg;
                lv_area_t ca = {cx, cy, cx + CELL_W - 1, cy + CELL_H - 1};
                lv_draw_rect(layer, &rd, &ca);
            }

            if (c.ch > ' ')
            {
                ld.color = fg;
                ld.unicode = c.ch;
                lv_point_t p = {cx, cy};
                lv_draw_letter(layer, &ld, &p);
            }
        }
    }
}

// --- UART <-> terminál -----------------------------------------------------

static void pump_serial()
{
    int budget = 1024;
    while (Serial.available() > 0 && budget-- > 0)
    {
        int b = Serial.read();
        if (b < 0)
            break;
        vt.feed((uint8_t)b);
    }
}

static void send_line(const char *t)
{
    if (!t)
        return;
    Serial.print(t);
    Serial.print(line_ending_suffix(cfg.lineEnding));
    if (cfg.localEcho)
    {
        vt.feedStr(t);
        vt.feed('\r');
        vt.feed('\n');
    }
    vt.scrollToBottom(); // válasz látható legyen
}

// Nyers byte-sorozat küldése (speciális billentyukhöz: nyilak, Ctrl-C, Esc...).
static void send_raw(const char *seq)
{
    if (!seq)
        return;
    Serial.write((const uint8_t *)seq, strlen(seq));
    vt.scrollToBottom();
}

// --- Képernyo-billentyuzet -------------------------------------------------

static void close_keyboard()
{
    if (kb_overlay)
    {
        lv_obj_delete(kb_overlay);
        kb_overlay = nullptr;
        input_ta = nullptr;
    }
}

static void kb_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_READY)
        send_line(lv_textarea_get_text(input_ta));
    close_keyboard();
}

static void show_keyboard()
{
    if (kb_overlay)
        return;
    const int32_t W = scrW();
    const int32_t H = scrH();
    int32_t ovH = (H * 7) / 10;
    if (ovH < 150)
        ovH = 150;
    const int32_t inH = 36;

    kb_overlay = lv_obj_create(lv_layer_top());
    lv_obj_remove_style_all(kb_overlay);
    lv_obj_set_size(kb_overlay, W, ovH);
    lv_obj_align(kb_overlay, LV_ALIGN_BOTTOM_MID, 0, 0);

    input_ta = lv_textarea_create(kb_overlay);
    lv_textarea_set_one_line(input_ta, true);
    lv_textarea_set_placeholder_text(input_ta, "command...");
    lv_obj_set_size(input_ta, W, inH);
    lv_obj_align(input_ta, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *kb = lv_keyboard_create(kb_overlay);
    lv_obj_set_size(kb, W, ovH - inH);
    lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_keyboard_set_textarea(kb, input_ta);
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_READY, NULL);
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_CANCEL, NULL);
}

// --- Beállítások -----------------------------------------------------------

static void close_settings()
{
    if (set_overlay)
    {
        lv_obj_delete(set_overlay);
        set_overlay = nullptr;
        dd_baud = dd_le = sw_echo = dd_rot = nullptr;
    }
}

static void settings_save_cb(lv_event_t *)
{
    uint8_t oldRot = cfg.rotation;
    cfg.baud = BAUD_OPTIONS[lv_dropdown_get_selected(dd_baud)];
    cfg.lineEnding = (uint8_t)lv_dropdown_get_selected(dd_le);
    cfg.localEcho = lv_obj_has_state(sw_echo, LV_STATE_CHECKED);
    cfg.rotation = (uint8_t)lv_dropdown_get_selected(dd_rot);

    config_save(cfg);
    Serial.updateBaudRate(cfg.baud);
    close_settings();

    if (cfg.rotation != oldRot)
        g_rebuild = true;
    else
        update_status();
}

static void settings_cancel_cb(lv_event_t *) { close_settings(); }

// A szövegbeíró billentyuzet a beállításokból hívható elo.
static void settings_keyboard_cb(lv_event_t *)
{
    close_settings();
    show_keyboard();
}

// A speciális (Fn) billentyuk a beállításokból hívhatók elo.
static void settings_keys_cb(lv_event_t *)
{
    close_settings();
    show_keys();
}

static void show_settings()
{
    if (set_overlay)
        return;

    set_overlay = lv_obj_create(lv_layer_top());
    lv_obj_set_size(set_overlay, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(set_overlay, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(set_overlay, LV_OPA_60, LV_PART_MAIN);
    lv_obj_set_style_border_width(set_overlay, 0, LV_PART_MAIN);
    lv_obj_clear_flag(set_overlay, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *panel = lv_obj_create(set_overlay);
    lv_obj_set_size(panel, imin(224, scrW() - 10), imin(300, scrH() - 10));
    lv_obj_center(panel);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x1c2530), LV_PART_MAIN);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    lv_obj_t *t = lv_label_create(panel);
    lv_label_set_text(t, LV_SYMBOL_SETTINGS "  Settings");
    lv_obj_set_style_text_color(t, lv_color_hex(0xFFD400), LV_PART_MAIN);
    lv_obj_set_style_text_font(t, &lv_font_montserrat_16, LV_PART_MAIN);

    lv_obj_t *lb1 = lv_label_create(panel);
    lv_label_set_text(lb1, "Baud:");
    lv_obj_set_style_text_color(lb1, lv_color_hex(0xD0D8E0), LV_PART_MAIN);
    dd_baud = lv_dropdown_create(panel);
    lv_dropdown_set_options(dd_baud, "9600\n19200\n38400\n57600\n115200\n230400");
    lv_dropdown_set_selected(dd_baud, baud_to_index(cfg.baud));
    lv_obj_set_width(dd_baud, LV_PCT(100));

    lv_obj_t *lb2 = lv_label_create(panel);
    lv_label_set_text(lb2, "Line ending (on send):");
    lv_obj_set_style_text_color(lb2, lv_color_hex(0xD0D8E0), LV_PART_MAIN);
    dd_le = lv_dropdown_create(panel);
    lv_dropdown_set_options(dd_le, "None\nLF (\\n)\nCRLF (\\r\\n)\nCR (\\r)");
    lv_dropdown_set_selected(dd_le, cfg.lineEnding <= 3 ? cfg.lineEnding : 1);
    lv_obj_set_width(dd_le, LV_PCT(100));

    lv_obj_t *row = lv_obj_create(panel);
    lv_obj_remove_style_all(row);
    lv_obj_set_size(row, LV_PCT(100), 36);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_t *lb3 = lv_label_create(row);
    lv_label_set_text(lb3, "Local echo");
    lv_obj_set_style_text_color(lb3, lv_color_hex(0xD0D8E0), LV_PART_MAIN);
    sw_echo = lv_switch_create(row);
    if (cfg.localEcho)
        lv_obj_add_state(sw_echo, LV_STATE_CHECKED);

    lv_obj_t *lb4 = lv_label_create(panel);
    lv_label_set_text(lb4, "Orientation:");
    lv_obj_set_style_text_color(lb4, lv_color_hex(0xD0D8E0), LV_PART_MAIN);
    dd_rot = lv_dropdown_create(panel);
    lv_dropdown_set_options(dd_rot, "Portrait (46 cols)\nLandscape (62 cols)");
    lv_dropdown_set_selected(dd_rot, cfg.rotation ? 1 : 0);
    lv_obj_set_width(dd_rot, LV_PCT(100));

    // Billentyuzet elohívása (alapból nem látszik)
    lv_obj_t *kbbtn = lv_button_create(panel);
    lv_obj_set_width(kbbtn, LV_PCT(100));
    lv_obj_add_event_cb(kbbtn, settings_keyboard_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *kbl = lv_label_create(kbbtn);
    lv_label_set_text(kbl, LV_SYMBOL_KEYBOARD "  Keyboard");
    lv_obj_center(kbl);

    // Speciális (Fn) billentyuk elohívása
    lv_obj_t *fnbtn = lv_button_create(panel);
    lv_obj_set_width(fnbtn, LV_PCT(100));
    lv_obj_add_event_cb(fnbtn, settings_keys_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *fnl = lv_label_create(fnbtn);
    lv_label_set_text(fnl, "Fn keys (arrows, Ctrl-C...)");
    lv_obj_center(fnl);

    lv_obj_t *btnrow = lv_obj_create(panel);
    lv_obj_remove_style_all(btnrow);
    lv_obj_set_size(btnrow, LV_PCT(100), 50);
    lv_obj_set_flex_flow(btnrow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btnrow, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_t *cancel = lv_button_create(btnrow);
    lv_obj_add_event_cb(cancel, settings_cancel_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *cl = lv_label_create(cancel);
    lv_label_set_text(cl, "Cancel");
    lv_obj_t *save = lv_button_create(btnrow);
    lv_obj_add_event_cb(save, settings_save_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *sl = lv_label_create(save);
    lv_label_set_text(sl, "Save");
}

// --- Speciális billentyuk (nyers küldés a Pi-nek) --------------------------

static lv_obj_t *keys_overlay = nullptr;

static void rawkey_cb(lv_event_t *e)
{
    send_raw((const char *)lv_event_get_user_data(e));
}

static void close_keys()
{
    if (keys_overlay)
    {
        lv_obj_delete(keys_overlay);
        keys_overlay = nullptr;
    }
}

static void keys_close_cb(lv_event_t *) { close_keys(); }

// Egy speciális gomb a panelre.
static void add_key(lv_obj_t *parent, const char *label, const char *seq)
{
    lv_obj_t *b = lv_button_create(parent);
    lv_obj_set_size(b, 50, 34);
    lv_obj_add_event_cb(b, rawkey_cb, LV_EVENT_CLICKED, (void *)seq);
    lv_obj_t *l = lv_label_create(b);
    lv_label_set_text(l, label);
    lv_obj_center(l);
}

static void show_keys()
{
    if (keys_overlay)
        return;
    const int32_t W = scrW();

    keys_overlay = lv_obj_create(lv_layer_top());
    lv_obj_set_size(keys_overlay, W, 150);
    lv_obj_align(keys_overlay, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(keys_overlay, lv_color_hex(0x1c2530), LV_PART_MAIN);
    lv_obj_set_flex_flow(keys_overlay, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(keys_overlay, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(keys_overlay, 4, LV_PART_MAIN);
    lv_obj_set_style_pad_gap(keys_overlay, 4, LV_PART_MAIN);

    add_key(keys_overlay, "Esc", "\x1b");
    add_key(keys_overlay, "Tab", "\t");
    add_key(keys_overlay, LV_SYMBOL_UP, "\x1b[A");
    add_key(keys_overlay, "^C", "\x03");
    add_key(keys_overlay, LV_SYMBOL_LEFT, "\x1b[D");
    add_key(keys_overlay, LV_SYMBOL_DOWN, "\x1b[B");
    add_key(keys_overlay, LV_SYMBOL_RIGHT, "\x1b[C");
    add_key(keys_overlay, "^D", "\x04");
    add_key(keys_overlay, "^Z", "\x1a");
    add_key(keys_overlay, "Bksp", "\x7f");
    add_key(keys_overlay, LV_SYMBOL_OK, "\r"); // Enter

    lv_obj_t *cl = lv_button_create(keys_overlay);
    lv_obj_set_size(cl, 50, 34);
    lv_obj_add_event_cb(cl, keys_close_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *cll = lv_label_create(cl);
    lv_label_set_text(cll, LV_SYMBOL_CLOSE);
    lv_obj_center(cll);
}

// --- Húzás-görgetés (scrollback) -------------------------------------------

static int s_dragY = 0, s_dragScroll = 0;

static void term_press_cb(lv_event_t *e)
{
    lv_indev_t *indev = lv_indev_active();
    if (!indev)
        return;
    lv_point_t p;
    lv_indev_get_point(indev, &p);

    if (lv_event_get_code(e) == LV_EVENT_PRESSED)
    {
        s_dragY = p.y;
        s_dragScroll = vt.scroll();
    }
    else // PRESSING
    {
        int lines = (p.y - s_dragY) / CELL_H; // lefelé húzás -> régebbi sorok
        vt.setScroll(s_dragScroll + lines);
        update_status();
        lv_obj_invalidate(term_obj);
    }
}

// --- Felso sáv gombjai -----------------------------------------------------

static void btn_cfg_cb(lv_event_t *) { show_settings(); }
static void btn_exit_cb(lv_event_t *) { return_to_launcher(); }

static void apply_rotation()
{
    lv_display_set_rotation(lv_display_get_default(),
                            cfg.rotation ? LV_DISPLAY_ROTATION_90
                                         : LV_DISPLAY_ROTATION_0);
}

// Néhány ESP32-2432S022C panelnél a kijelzot expliciten be kell kapcsolni
// (DISPON) az init után, különben csak fehér hátteret látni (nincs kép).
static void display_force_on()
{
    auto *panel = (esp_lcd_panel_handle_t)lv_display_get_user_data(
        lv_display_get_default());
    if (panel)
        esp_lcd_panel_disp_on_off(panel, true);
}

// Kis ikon-gomb a felso sáv jobb oldalára (x = jobbszéltol mért eltolás).
static void top_btn(lv_obj_t *parent, const char *sym, lv_event_cb_t cb, int x)
{
    lv_obj_t *b = lv_button_create(parent);
    lv_obj_set_size(b, 32, TOP_H - 2);
    lv_obj_align(b, LV_ALIGN_TOP_RIGHT, x, 0);
    lv_obj_set_style_pad_all(b, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(b, cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *l = lv_label_create(b);
    lv_label_set_text(l, sym);
    lv_obj_center(l);
}

static void build_ui()
{
    const int32_t W = scrW();
    const int32_t H = scrH();

    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);

    // Felso sáv: bal oldalt állapot, jobb oldalt beállítás + vissza gomb.
    status_lbl = lv_label_create(scr);
    lv_obj_set_style_text_color(status_lbl, lv_color_hex(0x6ff0a0), LV_PART_MAIN);
    lv_obj_set_style_text_font(status_lbl, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_align(status_lbl, LV_ALIGN_TOP_LEFT, 4, 4);
    top_btn(scr, LV_SYMBOL_LEFT, btn_exit_cb, -2);      // vissza (legjobbszél)
    top_btn(scr, LV_SYMBOL_SETTINGS, btn_cfg_cb, -38);  // beállítás

    // Terminál-objektum (egyedi rajzolás). Kis margó, hogy a szélso oszlopok/
    // sorok ne lógjanak le a kijelzorol.
    const int32_t PADH = 3, PADV = 2;
    const int32_t termW = W;
    const int32_t termH = H - TOP_H;
    term_obj = lv_obj_create(scr);
    lv_obj_remove_style_all(term_obj);
    lv_obj_set_pos(term_obj, 0, TOP_H);
    lv_obj_set_size(term_obj, termW, termH);
    lv_obj_set_style_bg_color(term_obj, Vt100::defaultBg(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(term_obj, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_left(term_obj, PADH, LV_PART_MAIN);
    lv_obj_set_style_pad_right(term_obj, PADH, LV_PART_MAIN);
    lv_obj_set_style_pad_top(term_obj, PADV, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(term_obj, PADV, LV_PART_MAIN);
    lv_obj_clear_flag(term_obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(term_obj, LV_OBJ_FLAG_CLICKABLE); // húzás-görgetéshez
    lv_obj_add_event_cb(term_obj, term_draw_cb, LV_EVENT_DRAW_MAIN_END, NULL);
    lv_obj_add_event_cb(term_obj, term_press_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(term_obj, term_press_cb, LV_EVENT_PRESSING, NULL);

    // A rács mérete a (paddinggal csökkentett) tartalom-területbol.
    vt.resize((termW - 2 * PADH) / CELL_W, (termH - 2 * PADV) / CELL_H);

    update_status();
}

static void rebuild_ui()
{
    apply_rotation();
    lv_obj_clean(lv_screen_active());
    build_ui();
}

void setup()
{
    config_load(cfg);
    Serial.begin(cfg.baud); // UART0 a Pi felé (P1: TX=GPIO1, RX=GPIO3)

    smartdisplay_init();
    display_force_on();
    apply_rotation();
    build_ui();

    vt.feedStr("VT100 terminal ready.\r\n");
    vt.feedStr("Pi -> P1 port (TX=GPIO1, RX=GPIO3, GND).\r\n\n");
}

static uint32_t last_tick = 0;
static uint32_t last_draw = 0;
static constexpr uint32_t REDRAW_MS = 40; // ~25 fps — a gyors kimenetet összevonja

void loop()
{
    uint32_t now = millis();
    lv_tick_inc(now - last_tick);
    last_tick = now;

    if (g_rebuild)
    {
        g_rebuild = false;
        rebuild_ui();
    }

    pump_serial();

    // Throttle: a bejövo adatot összevonjuk, és legfeljebb ~25 fps-sel rajzolunk
    // újra. Így a gyors konzol-kimenet gördülékeny marad (nem rajzol minden byte-ra).
    if (vt.dirty() && (now - last_draw) >= REDRAW_MS)
    {
        vt.clearDirty();
        last_draw = now;
        lv_obj_invalidate(term_obj);
    }

    lv_timer_handler();
    delay(2);
}
