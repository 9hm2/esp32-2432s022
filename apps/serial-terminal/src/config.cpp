#include "config.h"

#include <Preferences.h>

const uint32_t BAUD_OPTIONS[] = {9600, 19200, 38400, 57600, 115200, 230400};
const size_t BAUD_OPTIONS_COUNT = sizeof(BAUD_OPTIONS) / sizeof(BAUD_OPTIONS[0]);

static Preferences prefs;

void config_load(TermConfig &c)
{
    prefs.begin("serterm", true); // read-only
    c.baud = prefs.getULong("baud", 115200);
    c.lineEnding = prefs.getUChar("le", 1); // alapból LF
    c.localEcho = prefs.getBool("echo", false);
    c.rotation = prefs.getUChar("rot", 0); // alapból álló
    c.wifi = prefs.getBool("wifi", false);
    prefs.end();
}

void config_save(const TermConfig &c)
{
    prefs.begin("serterm", false);
    prefs.putULong("baud", c.baud);
    prefs.putUChar("le", c.lineEnding);
    prefs.putBool("echo", c.localEcho);
    prefs.putUChar("rot", c.rotation);
    prefs.putBool("wifi", c.wifi);
    prefs.end();
}

int baud_to_index(uint32_t baud)
{
    for (size_t i = 0; i < BAUD_OPTIONS_COUNT; i++)
        if (BAUD_OPTIONS[i] == baud)
            return (int)i;
    return 4; // 115200
}

const char *line_ending_suffix(uint8_t le)
{
    switch (le)
    {
    case 1:  return "\n";
    case 2:  return "\r\n";
    case 3:  return "\r";
    default: return "";
    }
}
