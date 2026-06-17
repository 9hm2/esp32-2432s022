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
    case OTA_BUSY:        return "busy";
    case OTA_ERR_OPEN:    return "cannot open file";
    case OTA_ERR_EMPTY:   return "empty file";
    case OTA_ERR_MAGIC:   return "not an ESP app image (bad magic)";
    case OTA_ERR_TOO_BIG: return "too big for ota_0 partition";
    case OTA_ERR_NO_PART: return "no ota_0 partition";
    case OTA_ERR_BEGIN:   return "esp_ota_begin error";
    case OTA_ERR_READ:    return "SD read error";
    case OTA_ERR_WRITE:   return "flash write error";
    case OTA_ERR_END:     return "image verify error (SHA-256?)";
    case OTA_ERR_SETBOOT: return "set boot partition error";
    default:              return "unknown error";
    }
}

// Lépésenkénti flashelés állapota.
static File s_file;
static esp_ota_handle_t s_handle = 0;
static const esp_partition_t *s_target = nullptr;
static uint32_t s_written = 0, s_total = 0;
static bool s_active = false;

uint32_t ota_written() { return s_written; }
uint32_t ota_total() { return s_total; }

OtaResult ota_begin(const AppEntry &app)
{
    s_active = false;
    s_target = esp_ota_get_next_update_partition(NULL); // factory fut -> ota_0
    if (!s_target)
        return OTA_ERR_NO_PART;

    s_file = SD.open(app.path.c_str(), FILE_READ);
    if (!s_file)
        return OTA_ERR_OPEN;

    s_total = (uint32_t)s_file.size();
    if (s_total == 0) { s_file.close(); return OTA_ERR_EMPTY; }
    if (s_total > s_target->size) { s_file.close(); return OTA_ERR_TOO_BIG; }
    if (s_file.peek() != ESP_IMAGE_MAGIC) { s_file.close(); return OTA_ERR_MAGIC; }

    if (esp_ota_begin(s_target, s_total, &s_handle) != ESP_OK)
    {
        s_file.close();
        return OTA_ERR_BEGIN;
    }
    s_written = 0;
    s_active = true;
    return OTA_OK;
}

OtaResult ota_step(uint32_t maxBytes)
{
    if (!s_active)
        return OTA_ERR_WRITE;

    uint32_t doneNow = 0;
    while (doneNow < maxBytes && s_written < s_total)
    {
        int n = s_file.read(s_buf, CHUNK);
        if (n < 0)
        {
            esp_ota_abort(s_handle); s_file.close(); s_active = false;
            return OTA_ERR_READ;
        }
        if (n == 0)
            break;
        if (esp_ota_write(s_handle, s_buf, n) != ESP_OK)
        {
            esp_ota_abort(s_handle); s_file.close(); s_active = false;
            return OTA_ERR_WRITE;
        }
        s_written += n;
        doneNow += n;
    }

    if (s_written >= s_total)
    {
        s_file.close();
        s_active = false;
        if (esp_ota_end(s_handle) != ESP_OK)
            return OTA_ERR_END;
        if (esp_ota_set_boot_partition(s_target) != ESP_OK)
            return OTA_ERR_SETBOOT;
        return OTA_OK; // teljesen kész
    }

    // Ha ebben a lépésben nem haladtunk, de még nincs vége -> csonka fájl.
    if (doneNow == 0)
    {
        esp_ota_abort(s_handle); s_file.close(); s_active = false;
        return OTA_ERR_READ;
    }

    return OTA_BUSY;
}

void ota_reboot()
{
    delay(150); // hogy a soros/UI üzenet kimenjen
    esp_restart();
}
