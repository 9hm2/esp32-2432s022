// Grafikus SD-kártyás bootloader / app-indító — ESP32-2432S022C
// ============================================================
// M2 mérföldkő: a launcher érintőképernyős LVGL listában mutatja az SD-kártyán
// lévő .bin app-fájlokat. A kiválasztás egyelőre csak visszajelez (a tényleges
// SD->flash futtatás az M3-ban jön).

#include <Arduino.h>
#include <SD.h>
#include <esp32_smartdisplay.h>
#include <esp_lcd_panel_ops.h>
#include <esp_ota_ops.h>
#include <esp_partition.h>

#include "ota_runner.h"
#include "sd_apps.h"
#include "ui/launcher_ui.h"

// Néhány ESP32-2432S022C panelnél a kijelzot az init után expliciten be kell
// kapcsolni (DISPON), különben csak fehér hátteret látni (háttérvilágítás, nincs
// kép). A smartdisplay a panel handle-t a display user_data-jában tárolja.
static void display_force_on()
{
    auto *panel = (esp_lcd_panel_handle_t)lv_display_get_user_data(
        lv_display_get_default());
    if (panel)
        esp_lcd_panel_disp_on_off(panel, true);
}

// A panel SD-kártya CS lába (board: TF_CS = GPIO5).
static constexpr uint8_t SD_CS = 5;

// Az app-fájlok könyvtára az SD-n; ha üres/nincs, a gyökeret nézzük.
static constexpr const char *APPS_DIR = "/apps";

static constexpr size_t MAX_APPS = 64;
static AppEntry apps[MAX_APPS];
static size_t app_count = 0;

// Diagnosztika a soros portra: futó + app-partíciók.
static void printPartitions()
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    if (running)
        Serial.printf("Running part. : %-8s @ 0x%06x (%u KB)\n", running->label,
                      (unsigned)running->address, (unsigned)(running->size / 1024));

    const esp_partition_t *ota0 = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
    if (ota0)
        Serial.printf("ota_0 (app)   : %-8s @ 0x%06x (%u KB)\n", ota0->label,
                      (unsigned)ota0->address, (unsigned)(ota0->size / 1024));
}

// A kiválasztott app flashelésére váró kérés (a tényleges munka a loop()-ban
// fut, nem az LVGL eseménykezelőben).
static AppEntry pending_app;
static volatile bool flash_requested = false;

// Akkor hívódik, amikor a felhasználó appot választ a listából.
static void onAppSelected(const AppEntry &app)
{
    Serial.printf("Selected: %s (%s) -> %s\n", app.name.c_str(),
                  humanSize(app.size).c_str(), app.path.c_str());
    pending_app = app;
    flash_requested = true;
}

// OTA folyamat -> progress bar frissítés.
static void onFlashProgress(uint32_t written, uint32_t total, void *)
{
    uint8_t pct = total ? (uint8_t)(((uint64_t)written * 100) / total) : 0;
    char buf[48];
    snprintf(buf, sizeof(buf), "%u%%  (%u/%u KB)", pct,
             (unsigned)(written / 1024), (unsigned)(total / 1024));
    launcher_ui_progress_update(pct, buf);
}

// A kért app beírása az ota_0-ba és átindítás rá (a loop()-ból hívva).
static void doFlashAndBoot()
{
    Serial.printf("Flashing: %s ...\n", pending_app.path.c_str());
    launcher_ui_progress_begin("Flashing...");

    OtaResult r = ota_flash_app(pending_app, onFlashProgress, nullptr);

    launcher_ui_progress_end();

    if (r == OTA_OK)
    {
        Serial.println("Flash OK -> rebooting into app.");
        launcher_ui_show_message("Done",
                                 "Loaded, starting app.\nRESET = back here.");
        lv_refr_now(NULL);
        ota_reboot(); // does not return
    }
    else
    {
        Serial.printf("Flash ERROR: %s\n", ota_result_str(r));
        char m[128];
        snprintf(m, sizeof(m), "Failed:\n%s", ota_result_str(r));
        launcher_ui_show_message("Failed", m);
    }
}

// Az SD aktuális állapota (hotplug).
static bool sd_mounted = false;

// (Újra)olvassa az app-listát a csatolt kártyáról.
static void sdRescan()
{
    app_count = scanApps(APPS_DIR, apps, MAX_APPS);
    if (app_count == 0)
        app_count = scanApps("/", apps, MAX_APPS);
    Serial.printf("SD: %u .bin file(s) found.\n", (unsigned)app_count);
    launcher_ui_set_apps(apps, app_count, onAppSelected);
}

// Hotplug-figyelés: behelyezéskor csatol + listáz, kivételkor leválaszt + jelez.
static void pollSd()
{
    if (!sd_mounted)
    {
        if (sdInit(SD_CS)) // sikeres -> kártya behelyezve
        {
            sd_mounted = true;
            Serial.printf("SD inserted (%llu MB).\n",
                          SD.cardSize() / (1024ull * 1024ull));
            sdRescan();
        }
    }
    else if (!sdPresent()) // eltunt -> kivették
    {
        sd_mounted = false;
        SD.end();
        Serial.println("SD removed.");
        launcher_ui_no_sd();
    }
}

void setup()
{
    Serial.begin(115200);
    delay(300);
    Serial.println("\n\n=== SD-bootloader (launcher) ===");
    printPartitions();

    // Kijelző + touch + LVGL (a demóval azonos út)
    smartdisplay_init();
    display_force_on();
    lv_display_set_rotation(lv_display_get_default(), LV_DISPLAY_ROTATION_0);

    launcher_ui_init();
    launcher_ui_no_sd(); // induló állapot, amíg a kártyát nem érzékeljük
}

static uint32_t last_tick = 0;
static uint32_t last_sd_check = 0;

void loop()
{
    uint32_t now = millis();
    if (last_tick == 0)
        last_tick = now;
    lv_tick_inc(now - last_tick);
    last_tick = now;
    lv_timer_handler();

    // SD hotplug-poll ~600 ms-onként (de nem flashelés közben).
    if (!flash_requested && now - last_sd_check > 600)
    {
        last_sd_check = now;
        pollSd();
    }

    // A flashelést a fo ciklusban végezzük (nem az esemenykezelőben).
    if (flash_requested)
    {
        flash_requested = false;
        doFlashAndBoot();
    }

    delay(5);
}
