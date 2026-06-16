/**
 * ESP32-2432S022C (CYD 2.2") - grafikus keretrendszer demó
 * --------------------------------------------------------
 * Stack: Arduino + LVGL 9 + esp32-smartdisplay
 *   - Kijelző: ST7789, 240x320, 8 bites i80 (8080) párhuzamos busz
 *   - Touch:   CST816S kapacitív, I2C (SDA=21, SCL=22)
 *
 * A demó bemutatja a teljes grafikus láncot:
 *   - kijelző init + háttérvilágítás vezérlés
 *   - érintés (LVGL input device a smartdisplay-en keresztül)
 *   - LVGL widgetek (címke, gomb számlálóval, csúszka, állapotsor)
 */

#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <esp_lcd_panel_ops.h>

static lv_obj_t *counter_label;   // gomb megnyomások száma
static lv_obj_t *status_label;    // futásidejű állapot (heap, uptime)
static uint32_t press_count = 0;

// --- Eseménykezelők -------------------------------------------------------

static void btn_clicked_cb(lv_event_t *e)
{
    press_count++;
    lv_label_set_text_fmt(counter_label, "Erintesek: %lu", (unsigned long)press_count);
}

static void brightness_changed_cb(lv_event_t *e)
{
    lv_obj_t *slider = (lv_obj_t *)lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);   // 0..100
    smartdisplay_lcd_set_backlight(value / 100.0f); // 0.0..1.0
}

// --- UI felépítés ---------------------------------------------------------

static void build_ui(void)
{
    lv_obj_t *screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x101418), LV_PART_MAIN);

    // Cím
    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "ESP32-2432S022C");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFD400), LV_PART_MAIN); // CYD sarga
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);

    lv_obj_t *subtitle = lv_label_create(screen);
    lv_label_set_text(subtitle, "LVGL 9 + smartdisplay\nST7789 i80 | CST816S touch");
    lv_obj_set_style_text_align(subtitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0xA0A8B0), LV_PART_MAIN);
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 44);

    // Számláló gomb (érintés teszt)
    lv_obj_t *btn = lv_button_create(screen);
    lv_obj_set_size(btn, 180, 56);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, -10);
    lv_obj_add_event_cb(btn, btn_clicked_cb, LV_EVENT_CLICKED, NULL);

    counter_label = lv_label_create(btn);
    lv_label_set_text(counter_label, "Erintesek: 0");
    lv_obj_center(counter_label);

    // Háttérvilágítás csúszka
    lv_obj_t *bl_label = lv_label_create(screen);
    lv_label_set_text(bl_label, "Hattervilagitas");
    lv_obj_set_style_text_color(bl_label, lv_color_hex(0xA0A8B0), LV_PART_MAIN);
    lv_obj_align(bl_label, LV_ALIGN_CENTER, 0, 60);

    lv_obj_t *slider = lv_slider_create(screen);
    lv_obj_set_width(slider, 180);
    lv_slider_set_range(slider, 5, 100);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 90);
    lv_obj_add_event_cb(slider, brightness_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // Állapotsor (lent)
    status_label = lv_label_create(screen);
    lv_obj_set_style_text_color(status_label, lv_color_hex(0x70C0FF), LV_PART_MAIN);
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_label_set_text(status_label, "indul...");
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -8);
}

// Periodikus állapotfrissítés (1 mp)
static void status_timer_cb(lv_timer_t *t)
{
    lv_label_set_text_fmt(status_label, "Free heap: %u B | Uptime: %lu s",
                          (unsigned)ESP.getFreeHeap(),
                          (unsigned long)(millis() / 1000));
}

// --- Arduino belépési pontok ---------------------------------------------

void setup(void)
{
    Serial.begin(115200);
    Serial.println("ESP32-2432S022C grafikus demo indul...");

    // Kijelző + touch inicializálás (50%-os fenyerovel indul)
    smartdisplay_init();

    // Néhány 2432S022C panelnél a kijelzot expliciten be kell kapcsolni (DISPON),
    // különben csak fehér hátteret látni (nincs kép).
    lv_display_t *display = lv_display_get_default();
    {
        auto *panel = (esp_lcd_panel_handle_t)lv_display_get_user_data(display);
        if (panel)
            esp_lcd_panel_disp_on_off(panel, true);
    }

    // Alapertelmezett tajolas: portrait (240x320 allo)
    lv_display_set_rotation(display, LV_DISPLAY_ROTATION_0);

    build_ui();
    lv_timer_create(status_timer_cb, 1000, NULL);
}

static uint32_t last_tick = 0;

void loop(void)
{
    uint32_t now = millis();
    lv_tick_inc(now - last_tick);
    last_tick = now;

    lv_timer_handler(); // LVGL feldolgozás (rajzolás + input)
    delay(5);
}
