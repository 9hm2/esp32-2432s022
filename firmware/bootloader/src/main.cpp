// Grafikus SD-kártyás bootloader / app-indító — ESP32-2432S022C
// ============================================================
// M2 mérföldkő: a launcher érintőképernyős LVGL listában mutatja az SD-kártyán
// lévő .bin app-fájlokat. A kiválasztás egyelőre csak visszajelez (a tényleges
// SD->flash futtatás az M3-ban jön).

#include <Arduino.h>
#include <SD.h>
#include <esp32_smartdisplay.h>
#include <esp_ota_ops.h>
#include <esp_partition.h>

#include "sd_apps.h"
#include "ui/launcher_ui.h"

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
    Serial.printf("Futo particio : %-8s @ 0x%06x (%u KB)\n", running->label,
                  (unsigned)running->address, (unsigned)(running->size / 1024));

    const esp_partition_t *ota0 = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
    if (ota0)
        Serial.printf("ota_0 (app)   : %-8s @ 0x%06x (%u KB)\n", ota0->label,
                      (unsigned)ota0->address, (unsigned)(ota0->size / 1024));
}

// Akkor hívódik, amikor a felhasználó appot választ a listából.
static void onAppSelected(const AppEntry &app)
{
    Serial.printf("Kivalasztva: %s (%s) -> %s\n", app.name.c_str(),
                  humanSize(app.size).c_str(), app.path.c_str());

    char msg[160];
    snprintf(msg, sizeof(msg),
             "%s\nMeret: %s\n\nA flashelés es futtatás az M3-ban jon.",
             app.name.c_str(), humanSize(app.size).c_str());
    launcher_ui_show_message("Kivalasztva", msg);
}

void setup()
{
    Serial.begin(115200);
    delay(300);
    Serial.println("\n\n=== SD-bootloader (launcher) - M2 ===");
    printPartitions();

    // Kijelző + touch + LVGL
    smartdisplay_init();
    lv_display_set_rotation(lv_display_get_default(), LV_DISPLAY_ROTATION_0);
    launcher_ui_init();

    // SD csatolás + app-keresés
    Serial.println("SD-kartya csatolasa...");
    if (!sdInit(SD_CS))
    {
        Serial.println("HIBA: az SD-kartya nem csatolhato.");
        launcher_ui_set_apps(nullptr, 0, onAppSelected);
        launcher_ui_show_message("Nincs SD-kartya",
                                 "Helyezz be egy FAT32 kartyat /apps/*.bin fajlokkal, majd RESET.");
        return;
    }
    Serial.printf("SD csatolva. Meret: %llu MB\n", SD.cardSize() / (1024ull * 1024ull));

    app_count = scanApps(APPS_DIR, apps, MAX_APPS);
    if (app_count == 0)
        app_count = scanApps("/", apps, MAX_APPS);

    Serial.printf("%u darab .bin talalva.\n", (unsigned)app_count);
    launcher_ui_set_apps(apps, app_count, onAppSelected);
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
