// SD-ről app-image (.bin) beírása az ota_0 partícióba és átindítás rá.
#pragma once

#include <Arduino.h>

#include "sd_apps.h"

// A flashelés eredménye.
enum OtaResult
{
    OTA_OK = 0,
    OTA_BUSY,         // még van hátra (lépésenkénti flashelés)
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

// Lépésenkénti flashelés (a loop()-ból hajtva), hogy a kijelzo közben
// frissülhessen (a renderelés NEM keveredhet a flash-írásokkal).
//   ota_begin(): megnyit + validál + esp_ota_begin. OTA_OK = sikeres indítás.
//   ota_step(): legfeljebb maxBytes-ot ír; OTA_BUSY = van hátra, OTA_OK = kész
//               (esp_ota_end + boot-partíció beállítva), egyéb = hiba.
OtaResult ota_begin(const AppEntry &app);
OtaResult ota_step(uint32_t maxBytes);
uint32_t ota_written();
uint32_t ota_total();

// Újraindítás (a beállított boot-partícióra).
void ota_reboot();

