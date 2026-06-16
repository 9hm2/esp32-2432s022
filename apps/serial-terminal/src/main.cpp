// Serial Terminal — Raspberry Pi soros konzol az ESP32-2432S022C kijelzon
// ======================================================================
// Valódi, látható soros terminál: az UART-on érkező szöveget görgethető
// terminálablakban mutatja, képernyő-billentyűzetrol parancsot küldhetsz, és a
// beállítások (baud, sorvég, helyi echo) állíthatók + NVS-be mentodnek.
//
// BEKÖTÉS a Raspberry Pi-hez (a panel P1 / "4P 1.25 Power supply base" portja):
//   - UART0-t használjuk (a P1-re kivezetett U0TXD=GPIO1, U0RXD=GPIO3).
//   - Pi TXD  -> board RXD (GPIO3)
//   - Pi RXD  <- board TXD (GPIO1)
//   - GND  <-> GND  (közös föld kötelezo)
//   - A Pi 3.3V logikai szintu (kompatibilis); 5V-ot NE köss a TX/RX-re.
//   - A panel táplálható a P1 Vin(5V)-rol vagy USB-rol.
//
// Megjegyzés: az UART0 a programozó/USB konzol is. Ez az app NEM ír debug
// üzenetet az UART0-ra, hogy ne zavarja a Pi-kapcsolatot.

#include <Arduino.h>
#include <esp32_smartdisplay.h>

#include "config.h"
#include "launcher_return.h"

static TermConfig cfg;

// --- UI elemek -------------------------------------------------------------
static lv_obj_t *term_ta = nullptr;   // a terminál-kimenet (read-only)
static lv_obj_t *status_lbl = nullptr; // felso állapotsor

// A keyboard / settings overlay-k.
static lv_obj_t *kb_overlay = nullptr;
static lv_obj_t *input_ta = nullptr;
static lv_obj_t *set_overlay = nullptr;
static lv_obj_t *dd_baud = nullptr;
static lv_obj_t *dd_le = nullptr;
static lv_obj_t *sw_echo = nullptr;

// --- Terminál-puffer -------------------------------------------------------
static String termBuf;
static bool termDirty = false;
static constexpr size_t TERM_MAX = 4000; // efölött trimmelünk
static constexpr size_t TERM_KEEP = 3000;

static void update_status()
{
    static const char *le_short[] = {"-", "LF", "CRLF", "CR"};
    lv_label_set_text_fmt(status_lbl, LV_SYMBOL_USB "  RPi  %lu 8N1  %s%s",
                          (unsigned long)cfg.baud,
                          le_short[cfg.lineEnding <= 3 ? cfg.lineEnding : 0],
                          cfg.localEcho ? "  echo" : "");
}

// Egy bejövo byte feldolgozása (ANSI/vezérlo-szuréssel) -> termBuf.
static void term_feed_byte(uint8_t b)
{
    static int esc = 0; // 0=normál, 1=ESC után, 2=CSI-ben

    if (esc == 1)
    {
        esc = (b == '[') ? 2 : 0; // csak a CSI ( ESC [ ) szekvenciát kezeljük
        return;
    }
    if (esc == 2)
    {
        if (b >= 0x40 && b <= 0x7E) // a CSI végjele egy betu/jel
            esc = 0;
        return; // a szekvencia tartalmát eldobjuk
    }

    if (b == 0x1B) { esc = 1; return; }        // ESC
    if (b == '\r') return;                       // CR-t eldobjuk (CRLF -> LF)
    if (b == 0x08 || b == 0x7F)                  // backspace / DEL
    {
        if (termBuf.length())
            termBuf.remove(termBuf.length() - 1);
        termDirty = true;
        return;
    }
    if (b == '\n') { termBuf += '\n'; termDirty = true; return; }
    if (b == '\t') { termBuf += "  "; termDirty = true; return; }
    if (b >= 0x20 && b < 0x7F) { termBuf += (char)b; termDirty = true; }
    // minden más (nem nyomtatható / >=0x80) eldobva
}

// A puffer megjelenítése + auto-görgetés a végére.
static void term_flush()
{
    if (!termDirty)
        return;
    termDirty = false;

    if (termBuf.length() > TERM_MAX)
    {
        int cut = (int)termBuf.length() - (int)TERM_KEEP;
        int nl = termBuf.indexOf('\n', cut);
        termBuf.remove(0, (nl >= 0 ? nl + 1 : cut));
    }

    lv_textarea_set_text(term_ta, termBuf.c_str());
    lv_textarea_set_cursor_pos(term_ta, LV_TEXTAREA_CURSOR_LAST); // a végére görget
}

// UART -> terminál (loop()-ból, korlátozott mennyiség ciklusonként).
static void pump_serial()
{
    int budget = 512;
    while (Serial.available() > 0 && budget-- > 0)
    {
        int b = Serial.read();
        if (b < 0)
            break;
        term_feed_byte((uint8_t)b);
    }
}

// Egy sor kiküldése a Pi felé.
static void send_line(const char *t)
{
    if (!t)
        return;
    Serial.print(t);
    Serial.print(line_ending_suffix(cfg.lineEnding));
    if (cfg.localEcho)
    {
        termBuf += t;
        termBuf += '\n';
        termDirty = true;
    }
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
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_READY) // pipa = küldés
        send_line(lv_textarea_get_text(input_ta));
    close_keyboard(); // READY és CANCEL esetén is bezár
}

static void show_keyboard()
{
    if (kb_overlay)
        return;

    kb_overlay = lv_obj_create(lv_layer_top());
    lv_obj_remove_style_all(kb_overlay);
    lv_obj_set_size(kb_overlay, 240, 200);
    lv_obj_align(kb_overlay, LV_ALIGN_BOTTOM_MID, 0, 0);

    input_ta = lv_textarea_create(kb_overlay);
    lv_textarea_set_one_line(input_ta, true);
    lv_textarea_set_placeholder_text(input_ta, "parancs...");
    lv_obj_set_size(input_ta, 240, 38);
    lv_obj_align(input_ta, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *kb = lv_keyboard_create(kb_overlay);
    lv_obj_set_size(kb, 240, 158);
    lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_keyboard_set_textarea(kb, input_ta);
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_READY, NULL);
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_CANCEL, NULL);
}

// --- Beállítások ablak -----------------------------------------------------

static void close_settings()
{
    if (set_overlay)
    {
        lv_obj_delete(set_overlay);
        set_overlay = nullptr;
        dd_baud = dd_le = sw_echo = nullptr;
    }
}

static void settings_save_cb(lv_event_t *)
{
    cfg.baud = BAUD_OPTIONS[lv_dropdown_get_selected(dd_baud)];
    cfg.lineEnding = (uint8_t)lv_dropdown_get_selected(dd_le);
    cfg.localEcho = lv_obj_has_state(sw_echo, LV_STATE_CHECKED);

    config_save(cfg);
    Serial.updateBaudRate(cfg.baud); // azonnali alkalmazás
    update_status();
    close_settings();
}

static void settings_cancel_cb(lv_event_t *) { close_settings(); }

static void show_settings()
{
    if (set_overlay)
        return;

    // Sötétíto háttér
    set_overlay = lv_obj_create(lv_layer_top());
    lv_obj_set_size(set_overlay, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(set_overlay, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(set_overlay, LV_OPA_60, LV_PART_MAIN);
    lv_obj_set_style_border_width(set_overlay, 0, LV_PART_MAIN);
    lv_obj_clear_flag(set_overlay, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *panel = lv_obj_create(set_overlay);
    lv_obj_set_size(panel, 224, 300);
    lv_obj_center(panel);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x1c2530), LV_PART_MAIN);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    lv_obj_t *t = lv_label_create(panel);
    lv_label_set_text(t, LV_SYMBOL_SETTINGS "  Beallitasok");
    lv_obj_set_style_text_color(t, lv_color_hex(0xFFD400), LV_PART_MAIN);
    lv_obj_set_style_text_font(t, &lv_font_montserrat_16, LV_PART_MAIN);

    // Baud
    lv_obj_t *lb1 = lv_label_create(panel);
    lv_label_set_text(lb1, "Baud:");
    lv_obj_set_style_text_color(lb1, lv_color_hex(0xD0D8E0), LV_PART_MAIN);
    dd_baud = lv_dropdown_create(panel);
    lv_dropdown_set_options(dd_baud, "9600\n19200\n38400\n57600\n115200\n230400");
    lv_dropdown_set_selected(dd_baud, baud_to_index(cfg.baud));
    lv_obj_set_width(dd_baud, LV_PCT(100));

    // Sorvég
    lv_obj_t *lb2 = lv_label_create(panel);
    lv_label_set_text(lb2, "Sorveg (kuldeskor):");
    lv_obj_set_style_text_color(lb2, lv_color_hex(0xD0D8E0), LV_PART_MAIN);
    dd_le = lv_dropdown_create(panel);
    lv_dropdown_set_options(dd_le, "Nincs\nLF (\\n)\nCRLF (\\r\\n)\nCR (\\r)");
    lv_dropdown_set_selected(dd_le, cfg.lineEnding <= 3 ? cfg.lineEnding : 1);
    lv_obj_set_width(dd_le, LV_PCT(100));

    // Helyi echo
    lv_obj_t *row = lv_obj_create(panel);
    lv_obj_remove_style_all(row);
    lv_obj_set_size(row, LV_PCT(100), 36);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_t *lb3 = lv_label_create(row);
    lv_label_set_text(lb3, "Helyi echo");
    lv_obj_set_style_text_color(lb3, lv_color_hex(0xD0D8E0), LV_PART_MAIN);
    sw_echo = lv_switch_create(row);
    if (cfg.localEcho)
        lv_obj_add_state(sw_echo, LV_STATE_CHECKED);

    // Gombok
    lv_obj_t *btnrow = lv_obj_create(panel);
    lv_obj_remove_style_all(btnrow);
    lv_obj_set_size(btnrow, LV_PCT(100), 50);
    lv_obj_set_flex_flow(btnrow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btnrow, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    lv_obj_t *cancel = lv_button_create(btnrow);
    lv_obj_add_event_cb(cancel, settings_cancel_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *cl = lv_label_create(cancel);
    lv_label_set_text(cl, "Megse");

    lv_obj_t *save = lv_button_create(btnrow);
    lv_obj_add_event_cb(save, settings_save_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *sl = lv_label_create(save);
    lv_label_set_text(sl, "Mentes");
}

// --- Alsó eszköztár gombjai ------------------------------------------------

static void btn_type_cb(lv_event_t *) { show_keyboard(); }
static void btn_cfg_cb(lv_event_t *) { show_settings(); }
static void btn_clear_cb(lv_event_t *)
{
    termBuf = "";
    lv_textarea_set_text(term_ta, "");
}
static void btn_exit_cb(lv_event_t *) { return_to_launcher(); }

static lv_obj_t *make_tool_btn(lv_obj_t *parent, const char *sym, lv_event_cb_t cb)
{
    lv_obj_t *b = lv_button_create(parent);
    lv_obj_set_flex_grow(b, 1);
    lv_obj_set_height(b, LV_PCT(100));
    lv_obj_add_event_cb(b, cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *l = lv_label_create(b);
    lv_label_set_text(l, sym);
    lv_obj_center(l);
    return b;
}

static void build_ui()
{
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x0a0e12), LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);

    // Állapotsor
    status_lbl = lv_label_create(scr);
    lv_obj_set_style_text_color(status_lbl, lv_color_hex(0x6ff0a0), LV_PART_MAIN);
    lv_obj_set_style_text_font(status_lbl, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_align(status_lbl, LV_ALIGN_TOP_LEFT, 4, 5);

    // Terminál-kimenet (read-only, monospace)
    term_ta = lv_textarea_create(scr);
    lv_obj_set_pos(term_ta, 0, 24);
    lv_obj_set_size(term_ta, 240, 254);
    lv_obj_remove_flag(term_ta, LV_OBJ_FLAG_CLICKABLE); // ne lehessen szerkeszteni
    lv_textarea_set_cursor_click_pos(term_ta, false);
    lv_obj_set_style_bg_color(term_ta, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_color(term_ta, lv_color_hex(0xC8F0C8), LV_PART_MAIN);
    lv_obj_set_style_text_font(term_ta, &lv_font_unscii_8, LV_PART_MAIN);
    lv_obj_set_style_pad_all(term_ta, 3, LV_PART_MAIN);

    // Alsó eszköztár
    lv_obj_t *bar = lv_obj_create(scr);
    lv_obj_remove_style_all(bar);
    lv_obj_set_size(bar, 240, 40);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(bar, 4, LV_PART_MAIN);
    lv_obj_set_style_pad_all(bar, 3, LV_PART_MAIN);

    make_tool_btn(bar, LV_SYMBOL_KEYBOARD, btn_type_cb);
    make_tool_btn(bar, LV_SYMBOL_SETTINGS, btn_cfg_cb);
    make_tool_btn(bar, LV_SYMBOL_TRASH, btn_clear_cb);
    make_tool_btn(bar, LV_SYMBOL_LEFT, btn_exit_cb);

    update_status();
}

void setup()
{
    config_load(cfg);

    // UART0 a Pi felé (P1: TX=GPIO1, RX=GPIO3). NINCS debug print ide.
    Serial.begin(cfg.baud);

    smartdisplay_init();
    lv_display_set_rotation(lv_display_get_default(), LV_DISPLAY_ROTATION_0);
    build_ui();

    termBuf.reserve(TERM_MAX + 64);
    termBuf = "[serial terminal]\n";
    termBuf += "Csatlakoztasd a Pi-t a P1 portra (TX=GPIO1, RX=GPIO3, GND).\n\n";
    termDirty = true;
}

static uint32_t last_tick = 0;

void loop()
{
    uint32_t now = millis();
    lv_tick_inc(now - last_tick);
    last_tick = now;

    pump_serial();
    term_flush();

    lv_timer_handler();
    delay(5);
}
