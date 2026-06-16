// A serial-terminal app perzisztens beállításai (NVS-ben tárolva).
#pragma once

#include <Arduino.h>

struct TermConfig
{
    uint32_t baud;      // soros sebesség
    uint8_t lineEnding; // 0=nincs, 1=LF(\n), 2=CRLF(\r\n), 3=CR(\r)
    bool localEcho;     // a beírt parancsot a saját kijelzon is mutassuk-e
    uint8_t rotation;   // 0=álló (240x320), 1=fekvo (320x240)
    bool wifi;          // Wi-Fi telnet bevitel (AP + telnet szerver)
};

// Betöltés/mentés NVS-bol (Preferences, "serterm" névtér).
void config_load(TermConfig &c);
void config_save(const TermConfig &c);

// Választható baud-értékek (a beállítások legördülojéhez).
extern const uint32_t BAUD_OPTIONS[];
extern const size_t BAUD_OPTIONS_COUNT;
int baud_to_index(uint32_t baud);

// A kiküldendo sorvég-byte-ok az adott lineEnding-hez.
const char *line_ending_suffix(uint8_t le);
