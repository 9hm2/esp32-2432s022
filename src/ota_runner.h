// SD-ről app-image (.bin) beírása az ota_0 partícióba és átindítás rá.
#pragma once

#include <Arduino.h>

#include "sd_apps.h"

// A flashelés eredménye.
enum OtaResult
{
    OTA_OK = 0,
    OTA_ERR_OPEN,     // a fájl nem nyitható meg
    OTA_ERR_EMPTY,    // üres fájl
    OTA_ERR_MAGIC,    // nem ESP app-image (első byte != 0xE9)
    OTA_ERR_TOO_BIG,  // nem fér az ota_0 partícióba
    OTA_ERR_NO_PART,  // nincs ota_0 partíció
    OTA_ERR_BEGIN,    // esp_ota_begin hiba
    OTA_ERR_READ,     // SD olvasási hiba
    OTA_ERR_WRITE,    // esp_ota_write hiba
    OTA_ERR_END,      // esp_ota_end hiba (pl. SHA-256 nem stimmel)
    OTA_ERR_SETBOOT,  // boot-partíció beállítása nem sikerült
};

// Ember által olvasható hibaszöveg.
const char *ota_result_str(OtaResult r);

// Folyamat-visszahívás (written/total byte). 'ctx' tetszőleges felhasználói adat.
typedef void (*ota_progress_cb_t)(uint32_t written, uint32_t total, void *ctx);

// A megadott app-image beírása az ota_0 partícióba és boot-partíciónak állítása.
// NEM indít újra (azt a hívó dönti el). OTA_OK esetén kész a boot.
OtaResult ota_flash_app(const AppEntry &app, ota_progress_cb_t cb, void *ctx);

// Újraindítás (a beállított boot-partícióra).
void ota_reboot();
