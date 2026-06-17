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

// Lépésenkénti flashelés állapota.
static bool flashing = false;

// A folyamatjelzo sávjának frissítése (csak %-változáskor); a tényleges
// renderelést a fo loop lv_timer_handler-e végzi.
static void updateFlashBar()
{
    uint32_t total = ota_total(), written = ota_written();
    uint8_t pct = total ? (uint8_t)(((uint64_t)written * 100) / total) : 0;
    static int lastPct = -1;
    if ((int)pct == lastPct)
        return;
    lastPct = pct;
    char buf[48];
    snprintf(buf, sizeof(buf), "%u%%  (%u/%u KB)", pct,
             (unsigned)(written / 1024), (unsigned)(total / 1024));
    launcher_ui_progress_update(pct, buf);
}

// A flashelést a loop() hajtja lépésenként (lásd lent), hogy a kijelzo
// frissülhessen, és a render sose keveredjen a flash-írásokkal.
static void flashErrorUI(OtaResult r)
{
    Serial.printf("Flash ERROR: %s\n", ota_result_str(r));
    flashing = false;
    launcher_ui_progress_end();
    char m[128];
    snprintf(m, sizeof(m), "Failed:\n%s", ota_result_str(r));
    launcher_ui_show_message("Failed", m);
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
    lv_timer_handler(); // itt rajzol az LVGL (a folyamatjelzo is)

    // SD hotplug-poll ~600 ms-onként (de nem flashelés közben/elott).
    if (!flash_requested && !flashing && now - last_sd_check > 600)
    {
        last_sd_check = now;
        pollSd();
    }

    // App kiválasztva -> flashelés indítása (lépésenként).
    if (flash_requested && !flashing)
    {
        flash_requested = false;
        Serial.printf("Flashing: %s ...\n", pending_app.path.c_str());
        OtaResult r = ota_begin(pending_app);
        if (r != OTA_OK)
            flashErrorUI(r);
        else
        {
            flashing = true;
            launcher_ui_progress_begin("Flashing app...");
        }
    }

    // Lépésenkénti flashelés: egy adag (a render a loop elején történik, így a
    // kijelzo-flush sose esik egybe a flash-írásokkal).
    if (flashing)
    {
        OtaResult r = ota_step(48 * 1024); // ~48 KB / iteráció
        updateFlashBar();
        if (r == OTA_OK)
        {
            Serial.println("Flash OK -> rebooting into app.");
            ota_reboot(); // nem ter vissza
        }
        else if (r != OTA_BUSY)
            flashErrorUI(r);
    }

    delay(flashing ? 1 : 5);
}
