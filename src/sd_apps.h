// SD-kártya kezelés és .bin app-fájlok keresése.
// M1: csak felderítés + listázás; a flashelés (OTA) később (M3+) jön.
#pragma once

#include <Arduino.h>

// Egy talált app-fájl leírója.
struct AppEntry {
    String name;   // fájlnév kiterjesztéssel (pl. "game.bin")
    String path;   // teljes elérési út (pl. "/apps/game.bin")
    uint32_t size; // fájlméret byte-ban
};

// SD-kártya inicializálása a megadott CS lábon (a board szerint GPIO5).
// A többi SPI láb a board alapértelmezett VSPI kiosztása: SCK=18, MISO=19, MOSI=23.
// true, ha a kártya sikeresen csatolva lett.
bool sdInit(uint8_t csPin);

// Gyors ellenorzes, hogy a kártya jelen van-e (hotplug figyeléshez).
bool sdPresent();

// .bin fájlok keresése a megadott könyvtárban (nem rekurzív).
// A találatokat az 'out' tömbbe írja, legfeljebb 'maxOut' darabot.
// Visszaadja a ténylegesen megtalált fájlok számát.
size_t scanApps(const char *dir, AppEntry *out, size_t maxOut);

// Ember által olvasható méret (pl. "1.8 MB", "240 KB").
String humanSize(uint32_t bytes);
