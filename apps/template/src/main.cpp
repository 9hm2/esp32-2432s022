// Példa-/sablon alkalmazás a grafikus SD-launcherhez — ESP32-2432S022C
// ====================================================================
// Ez egy teljes, működő app, amit a launcher SD-ről betölt és futtat. Mutatja:
//   - a szokásos LVGL + smartdisplay grafikát,
//   - egy "Vissza a launcherhez" gombot (kooperatív visszalépés),
//   - és hogy RESET-re is visszatér a menübe (rollback).
//
// A lefordult firmware.bin-t másold az SD /apps/ mappájába (lásd README).

#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <esp_lcd_panel_ops.h>

#include "launcher_return.h"

// Néhány ESP32-2432S022C panelnél a kijelzot az init után expliciten be kell
// kapcsolni (DISPON), különben csak fehér hátteret látni (nincs kép).
static void display_force_on()
{
    auto *panel = (esp_lcd_panel_handle_t)lv_display_get_user_data(
        lv_display_get_default());
    if (panel)
        esp_lcd_panel_disp_on_off(panel, true);
}

static void back_btn_cb(lv_event_t *)
{
    return_to_launcher(); // factory partíció -> újraindítás
}

static void build_ui()
{
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x10241c), LV_PART_MAIN);

    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Demo app");
    lv_obj_set_style_text_color(title, lv_color_hex(0x6ff0a0), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    lv_obj_t *info = lv_label_create(scr);
    lv_label_set_text(info, "Az SD-rol betoltve\n(ota_0 particio)");
    lv_obj_set_style_text_align(info, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_color(info, lv_color_hex(0xA0A8B0), LV_PART_MAIN);
    lv_obj_align(info, LV_ALIGN_CENTER, 0, -30);

    // Vissza gomb (kooperatív)
    lv_obj_t *back = lv_button_create(scr);
    lv_obj_set_size(back, 200, 56);
    lv_obj_align(back, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_event_cb(back, back_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *bl = lv_label_create(back);
    lv_label_set_text(bl, LV_SYMBOL_LEFT "  Vissza a launcherhez");
    lv_obj_center(bl);

    lv_obj_t *hint = lv_label_create(scr);
    lv_label_set_text(hint, "(vagy nyomj RESET-et)");
    lv_obj_set_style_text_color(hint, lv_color_hex(0x70C0FF), LV_PART_MAIN);
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -10);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("\nDemo app fut (ota_0).");

    // FONTOS: NE hivd az esp_ota_mark_app_valid_cancel_rollback()-ot, hogy a
    // RESET-es rollback (vissza a launcherhez) is mukodjon.

    smartdisplay_init();
    display_force_on();
    lv_display_set_rotation(lv_display_get_default(), LV_DISPLAY_ROTATION_0);
    build_ui();
}

static uint32_t last_tick = 0;

void loop()
{
    uint32_t now = millis();
    lv_tick_inc(now - last_tick);
    last_tick = now;
    lv_timer_handler();
    delay(5);
}
