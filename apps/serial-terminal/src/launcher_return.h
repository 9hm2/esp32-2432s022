// Visszatérés a grafikus launcherhez (factory partíció).
// Tedd ezt egy gombra/eseményre az appodban a kooperatív visszalépéshez.
//
// Megjegyzés: a launcher úgy indítja az appot, hogy a rollback élesítve van
// (az app "pending verify" állapotban fut), ezért EGYSZERŰ RESET is visszavisz a
// menübe — feltéve, hogy az app NEM hívja az esp_ota_mark_app_valid...()-ot.
#pragma once

#include <esp_ota_ops.h>
#include <esp_partition.h>
#include <esp_system.h>

// A factory (launcher) partíciót állítja boot-célnak, majd újraindít.
inline void return_to_launcher()
{
    const esp_partition_t *factory = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, NULL);
    if (factory)
        esp_ota_set_boot_partition(factory);
    esp_restart();
}
