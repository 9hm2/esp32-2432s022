#include "launcher_ui.h"

#include <esp32_smartdisplay.h>

// A UI állapota.
static const AppEntry *s_apps = nullptr;
static size_t s_app_count = 0;
static app_selected_cb_t s_cb = nullptr;

static lv_obj_t *s_list = nullptr;   // a görgethető applista
static lv_obj_t *s_status = nullptr; // alsó állapotsor

// Folyamatjelző overlay elemei.
static lv_obj_t *s_prog_overlay = nullptr;
static lv_obj_t *s_prog_bar = nullptr;
static lv_obj_t *s_prog_label = nullptr;

// --- Egy listaelem megnyomása ---------------------------------------------

static void app_button_clicked_cb(lv_event_t *e)
{
    // Az appindexet a gomb user_data-jában tároljuk.
    size_t idx = (size_t)(uintptr_t)lv_event_get_user_data(e);
    if (s_cb && s_apps && idx < s_app_count)
        s_cb(s_apps[idx]);
}

// --- Üzenet OK gomb -> overlay törlése -------------------------------------

static void msg_ok_clicked_cb(lv_event_t *e)
{
    lv_obj_t *overlay = (lv_obj_t *)lv_event_get_user_data(e);
    if (overlay)
        lv_obj_delete(overlay);
}

// --- Publikus API ----------------------------------------------------------

void launcher_ui_init()
{
    lv_obj_t *screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x101418), LV_PART_MAIN);
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN);

    // Fejléc
    lv_obj_t *header = lv_obj_create(screen);
    lv_obj_set_size(header, LV_PCT(100), 40);
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_radius(header, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(header, lv_color_hex(0x1c2530), LV_PART_MAIN);
    lv_obj_set_style_border_width(header, 0, LV_PART_MAIN);
    lv_obj_clear_flag(header, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *title = lv_label_create(header);
    lv_label_set_text(title, LV_SYMBOL_DRIVE "  App launcher");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFD400), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_center(title);

    // Lista (a fejléc és az állapotsor közötti terület)
    s_list = lv_list_create(screen);
    lv_obj_set_size(s_list, LV_PCT(100), 240);
    lv_obj_align(s_list, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_set_style_bg_color(s_list, lv_color_hex(0x101418), LV_PART_MAIN);
    lv_obj_set_style_border_width(s_list, 0, LV_PART_MAIN);

    // Állapotsor (lent)
    s_status = lv_label_create(screen);
    lv_obj_set_style_text_color(s_status, lv_color_hex(0x70C0FF), LV_PART_MAIN);
    lv_obj_set_style_text_font(s_status, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_label_set_text(s_status, "starting...");
    lv_obj_align(s_status, LV_ALIGN_BOTTOM_MID, 0, -6);
}

void launcher_ui_set_apps(const AppEntry *apps, size_t count, app_selected_cb_t cb)
{
    s_apps = apps;
    s_app_count = count;
    s_cb = cb;

    lv_obj_clean(s_list); // korábbi elemek törlése

    if (count == 0)
    {
        lv_obj_t *empty = lv_label_create(s_list);
        lv_label_set_text(empty, LV_SYMBOL_WARNING "  No .bin on SD card");
        lv_obj_set_style_text_color(empty, lv_color_hex(0xA0A8B0), LV_PART_MAIN);
        lv_label_set_text(s_status, "0 apps");
        return;
    }

    for (size_t i = 0; i < count; i++)
    {
        // "nev   (meret)" felirat a gombon
        String label = apps[i].name + "   (" + humanSize(apps[i].size) + ")";
        lv_obj_t *btn = lv_list_add_button(s_list, LV_SYMBOL_FILE, label.c_str());
        lv_obj_add_event_cb(btn, app_button_clicked_cb, LV_EVENT_CLICKED,
                            (void *)(uintptr_t)i);
    }

    lv_label_set_text_fmt(s_status, "%u app(s)  -  tap to run", (unsigned)count);
}

void launcher_ui_no_sd()
{
    s_apps = nullptr;
    s_app_count = 0;
    lv_obj_clean(s_list);
    lv_obj_t *l = lv_label_create(s_list);
    lv_label_set_text(l, LV_SYMBOL_SD_CARD "  No SD card\nInsert it to list apps");
    lv_obj_set_style_text_color(l, lv_color_hex(0xA0A8B0), LV_PART_MAIN);
    lv_label_set_text(s_status, "no SD card");
}

void launcher_ui_show_message(const char *title, const char *msg)
{
    // Félig átlátszó háttér a felső rétegen (modális hatás).
    lv_obj_t *overlay = lv_obj_create(lv_layer_top());
    lv_obj_set_size(overlay, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(overlay, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(overlay, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_border_width(overlay, 0, LV_PART_MAIN);
    lv_obj_clear_flag(overlay, LV_OBJ_FLAG_SCROLLABLE);

    // A tényleges üzenetpanel
    lv_obj_t *panel = lv_obj_create(overlay);
    lv_obj_set_size(panel, 210, 170);
    lv_obj_center(panel);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x1c2530), LV_PART_MAIN);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    lv_obj_t *t = lv_label_create(panel);
    lv_label_set_text(t, title);
    lv_obj_set_style_text_color(t, lv_color_hex(0xFFD400), LV_PART_MAIN);
    lv_obj_set_style_text_font(t, &lv_font_montserrat_16, LV_PART_MAIN);

    lv_obj_t *m = lv_label_create(panel);
    lv_label_set_text(m, msg);
    lv_label_set_long_mode(m, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(m, LV_PCT(100));
    lv_obj_set_flex_grow(m, 1);
    lv_obj_set_style_text_color(m, lv_color_hex(0xD0D8E0), LV_PART_MAIN);

    lv_obj_t *ok = lv_button_create(panel);
    lv_obj_set_width(ok, LV_PCT(100));
    lv_obj_add_event_cb(ok, msg_ok_clicked_cb, LV_EVENT_CLICKED, overlay);
    lv_obj_t *ok_lbl = lv_label_create(ok);
    lv_label_set_text(ok_lbl, "OK");
    lv_obj_center(ok_lbl);
}

void launcher_ui_progress_begin(const char *title)
{
    launcher_ui_progress_end(); // ha maradt volna korábbi

    s_prog_overlay = lv_obj_create(lv_layer_top());
    lv_obj_set_size(s_prog_overlay, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(s_prog_overlay, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_prog_overlay, LV_OPA_60, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_prog_overlay, 0, LV_PART_MAIN);
    lv_obj_clear_flag(s_prog_overlay, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *panel = lv_obj_create(s_prog_overlay);
    lv_obj_set_size(panel, 210, 150);
    lv_obj_center(panel);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x1c2530), LV_PART_MAIN);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    lv_obj_t *t = lv_label_create(panel);
    lv_label_set_text(t, title);
    lv_obj_set_style_text_color(t, lv_color_hex(0xFFD400), LV_PART_MAIN);
    lv_obj_set_style_text_font(t, &lv_font_montserrat_16, LV_PART_MAIN);

    s_prog_bar = lv_bar_create(panel);
    lv_obj_set_size(s_prog_bar, LV_PCT(100), 18);
    lv_bar_set_range(s_prog_bar, 0, 100);
    lv_bar_set_value(s_prog_bar, 0, LV_ANIM_OFF);

    s_prog_label = lv_label_create(panel);
    lv_label_set_text(s_prog_label, "0%");
    lv_obj_set_style_text_color(s_prog_label, lv_color_hex(0xD0D8E0), LV_PART_MAIN);

    lv_refr_now(NULL);
}

void launcher_ui_progress_update(uint8_t pct, const char *text)
{
    if (!s_prog_overlay)
        return;
    if (pct > 100)
        pct = 100;
    lv_bar_set_value(s_prog_bar, pct, LV_ANIM_OFF);
    lv_label_set_text(s_prog_label, text);
    // NINCS lv_refr_now: a renderelést a fo loop lv_timer_handler-e végzi, így a
    // kijelzo-flush sosem keveredik a flash-írásokkal (az okozta a befagyást).
}

void launcher_ui_progress_end()
{
    if (s_prog_overlay)
    {
        lv_obj_delete(s_prog_overlay);
        s_prog_overlay = nullptr;
        s_prog_bar = nullptr;
        s_prog_label = nullptr;
    }
}
