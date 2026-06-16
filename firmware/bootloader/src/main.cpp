// Grafikus SD-kártyás bootloader / app-indító — ESP32-2432S022C
// ============================================================
// M1 mérföldkő: a launcher (factory) bebootol, csatolja az SD-kártyát, és a
// soros porton kilistázza a rajta lévő .bin app-fájlokat. A GUI (LVGL) az M2-ben
// jön, a tényleges SD->flash futtatás (OTA) az M3+-ban.

#include <Arduino.h>
#include <SD.h>
#include <esp_ota_ops.h>
#include <esp_partition.h>

#include "sd_apps.h"

// A panel SD-kártya CS lába (a board-definíció szerint TF_CS = GPIO5).
static constexpr uint8_t SD_CS = 5;

// A keresett app-fájlok könyvtára az SD-n; ha nincs, a gyökeret nézzük.
static constexpr const char *APPS_DIR = "/apps";

static constexpr size_t MAX_APPS = 64;
static AppEntry apps[MAX_APPS];

// Diagnosztika: kiírja a futó és az app-partíciókat, hogy lássuk, a
// partíciótábla rendben betöltődött-e.
static void printPartitions()
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    Serial.printf("Futo particio : %-8s @ 0x%06x (%u KB)\n",
                  running->label, (unsigned)running->address,
                  (unsigned)(running->size / 1024));

    const esp_partition_t *factory = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, NULL);
    if (factory)
        Serial.printf("factory       : %-8s @ 0x%06x (%u KB)\n",
                      factory->label, (unsigned)factory->address,
                      (unsigned)(factory->size / 1024));

    const esp_partition_t *ota0 = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
    if (ota0)
        Serial.printf("ota_0 (app)   : %-8s @ 0x%06x (%u KB)  <- ide kerul majd az app\n",
                      ota0->label, (unsigned)ota0->address,
                      (unsigned)(ota0->size / 1024));
}

// Kilistázza az adott könyvtár .bin fájljait; visszaadja a darabszámot.
static size_t listDir(const char *dir)
{
    size_t n = scanApps(dir, apps, MAX_APPS);
    if (n == 0)
        return 0;

    Serial.printf("\n%u darab .bin fajl a(z) '%s' konyvtarban:\n", (unsigned)n, dir);
    Serial.println("-----------------------------------------------");
    for (size_t i = 0; i < n; i++)
        Serial.printf("  [%2u] %-28s %10s\n", (unsigned)i,
                      apps[i].name.c_str(), humanSize(apps[i].size).c_str());
    Serial.println("-----------------------------------------------");
    return n;
}

void setup()
{
    Serial.begin(115200);
    delay(300);
    Serial.println("\n\n=== SD-bootloader (launcher) - M1 ===");

    printPartitions();

    Serial.println("\nSD-kartya csatolasa...");
    if (!sdInit(SD_CS))
    {
        Serial.println("HIBA: az SD-kartya nem csatolhato (nincs kartya, vagy rossz formatum/bekotes).");
        return;
    }
    Serial.printf("SD csatolva. Meret: %llu MB\n", SD.cardSize() / (1024ull * 1024ull));

    // Elobb a dedikalt /apps konyvtar, ha ott nincs semmi, a gyoker.
    size_t found = listDir(APPS_DIR);
    if (found == 0)
    {
        Serial.printf("A(z) '%s' ures vagy nem letezik; a gyoker konyvtar nezese...\n", APPS_DIR);
        found = listDir("/");
    }

    if (found == 0)
        Serial.println("Nem talalhato .bin fajl az SD-kartyan.");

    Serial.println("\nM1 kesz: a listazas megtortent. (GUI = M2)");
}

void loop()
{
    // M1: nincs futo feladat. A GUI esemenykezeles az M2-ben kerul ide.
    delay(1000);
}
