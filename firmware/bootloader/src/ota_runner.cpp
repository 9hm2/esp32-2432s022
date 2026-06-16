#include "ota_runner.h"

#include <SD.h>
#include <esp_ota_ops.h>
#include <esp_partition.h>

// Az ESP32 alkalmazás-image első byte-ja (image header magic).
static constexpr uint8_t ESP_IMAGE_MAGIC = 0xE9;

// Olvasási blokkméret SD-ről flashbe. 4 KB jó kompromisszum sebesség/RAM közt.
static constexpr size_t CHUNK = 4096;
static uint8_t s_buf[CHUNK];

const char *ota_result_str(OtaResult r)
{
    switch (r)
    {
    case OTA_OK:          return "OK";
    case OTA_ERR_OPEN:    return "a fajl nem nyithato meg";
    case OTA_ERR_EMPTY:   return "ures fajl";
    case OTA_ERR_MAGIC:   return "nem ESP app-image (rossz magic)";
    case OTA_ERR_TOO_BIG: return "tul nagy az ota_0 particiohoz";
    case OTA_ERR_NO_PART: return "nincs ota_0 particio";
    case OTA_ERR_BEGIN:   return "esp_ota_begin hiba";
    case OTA_ERR_READ:    return "SD olvasasi hiba";
    case OTA_ERR_WRITE:   return "flash iras hiba";
    case OTA_ERR_END:     return "image ellenorzes hiba (SHA-256?)";
    case OTA_ERR_SETBOOT: return "boot-particio beallitas hiba";
    default:              return "ismeretlen hiba";
    }
}

OtaResult ota_flash_app(const AppEntry &app, ota_progress_cb_t cb, void *ctx)
{
    // 1) Cél-partíció (a következő OTA slot; factory fut -> ota_0)
    const esp_partition_t *target = esp_ota_get_next_update_partition(NULL);
    if (!target)
        return OTA_ERR_NO_PART;

    // 2) Fájl megnyitása + alap-validáció
    File f = SD.open(app.path.c_str(), FILE_READ);
    if (!f)
        return OTA_ERR_OPEN;

    uint32_t total = (uint32_t)f.size();
    if (total == 0)
    {
        f.close();
        return OTA_ERR_EMPTY;
    }
    if (total > target->size)
    {
        f.close();
        return OTA_ERR_TOO_BIG;
    }

    int first = f.peek();
    if (first != ESP_IMAGE_MAGIC)
    {
        f.close();
        return OTA_ERR_MAGIC;
    }

    // 3) OTA írás indítása (a partíció törlése ezen belül megtörténik)
    esp_ota_handle_t handle = 0;
    if (esp_ota_begin(target, total, &handle) != ESP_OK)
    {
        f.close();
        return OTA_ERR_BEGIN;
    }

    // 4) Másolás chunkokban SD -> flash
    uint32_t written = 0;
    if (cb)
        cb(0, total, ctx);

    while (written < total)
    {
        int n = f.read(s_buf, CHUNK);
        if (n < 0)
        {
            esp_ota_abort(handle);
            f.close();
            return OTA_ERR_READ;
        }
        if (n == 0)
            break; // fájl vége a vártnál korábban

        if (esp_ota_write(handle, s_buf, n) != ESP_OK)
        {
            esp_ota_abort(handle);
            f.close();
            return OTA_ERR_WRITE;
        }

        written += n;
        if (cb)
            cb(written, total, ctx);
    }

    f.close();

    // 5) Lezárás (image-integritás + SHA-256 ellenőrzés)
    esp_err_t end_err = esp_ota_end(handle);
    if (end_err != ESP_OK)
        return OTA_ERR_END;

    // 6) Boot-partíció beállítása az új appra
    if (esp_ota_set_boot_partition(target) != ESP_OK)
        return OTA_ERR_SETBOOT;

    return OTA_OK;
}

void ota_reboot()
{
    delay(150); // hogy a soros/UI üzenet kimenjen
    esp_restart();
}
