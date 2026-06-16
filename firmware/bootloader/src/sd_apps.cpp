#include "sd_apps.h"

#include <SD.h>
#include <SPI.h>

// A board VSPI alapértelmezett lábai (megegyeznek a panel SD bekötésével).
static constexpr int8_t SD_SCK = 18;
static constexpr int8_t SD_MISO = 19;
static constexpr int8_t SD_MOSI = 23;

bool sdInit(uint8_t csPin)
{
    // Explicit SPI init, hogy biztosan a helyes lábakat használjuk.
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, csPin);
    if (!SD.begin(csPin, SPI))
        return false;

    return SD.cardType() != CARD_NONE;
}

// Kisbetűsíti és ellenőrzi, hogy ".bin"-re végződik-e a név.
static bool isBin(const String &name)
{
    if (name.length() < 4)
        return false;
    String ext = name.substring(name.length() - 4);
    ext.toLowerCase();
    return ext == ".bin";
}

// Az openNextFile() verziófüggően teljes utat vagy csak nevet ad — csak a
// fájlnév-részt tartjuk meg (az utolsó '/' utáni rész).
static String baseName(const String &path)
{
    int slash = path.lastIndexOf('/');
    return slash >= 0 ? path.substring(slash + 1) : path;
}

size_t scanApps(const char *dir, AppEntry *out, size_t maxOut)
{
    File root = SD.open(dir);
    if (!root || !root.isDirectory())
        return 0;

    size_t count = 0;
    for (File entry = root.openNextFile(); entry; entry = root.openNextFile())
    {
        if (!entry.isDirectory())
        {
            String name = baseName(String(entry.name()));
            if (isBin(name) && count < maxOut)
            {
                out[count].name = name;
                // Teljes út összeállítása (a könyvtár + '/' + név), dupla '/' nélkül.
                String base(dir);
                if (!base.endsWith("/"))
                    base += "/";
                out[count].path = base + name;
                out[count].size = (uint32_t)entry.size();
                count++;
            }
        }
        entry.close();
    }
    root.close();
    return count;
}

String humanSize(uint32_t bytes)
{
    char buf[24];
    if (bytes >= 1024u * 1024u)
        snprintf(buf, sizeof(buf), "%.1f MB", bytes / (1024.0 * 1024.0));
    else if (bytes >= 1024u)
        snprintf(buf, sizeof(buf), "%.1f KB", bytes / 1024.0);
    else
        snprintf(buf, sizeof(buf), "%lu B", (unsigned long)bytes);
    return String(buf);
}
