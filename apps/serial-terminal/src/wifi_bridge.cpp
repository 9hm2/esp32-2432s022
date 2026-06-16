#include "wifi_bridge.h"

#include <WiFi.h>

// A csatlakozandó hálózat.
#define WIFI_SSID "HM"
#define WIFI_PASS "x2ta9zgu"
#define TELNET_PORT 23

static WiFiServer s_server(TELNET_PORT);
static WiFiClient s_client;
static bool s_running = false;   // a Wi-Fi funkció be van-e kapcsolva
static bool s_serverUp = false;  // a telnet szerver fut-e (csak kapcsolódva)
static int s_iac = 0;            // telnet IAC parser-állapot

void wifi_bridge_start()
{
    if (s_running)
        return;
    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); // kisebb késleltetés
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    s_running = true;
    s_serverUp = false;
}

void wifi_bridge_stop()
{
    if (!s_running)
        return;
    if (s_client)
        s_client.stop();
    if (s_serverUp)
        s_server.end();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    s_running = false;
    s_serverUp = false;
}

bool wifi_bridge_running() { return s_running; }
bool wifi_bridge_connected() { return s_running && WiFi.status() == WL_CONNECTED; }

String wifi_bridge_ip()
{
    if (!wifi_bridge_connected())
        return String();
    return WiFi.localIP().toString();
}

void wifi_bridge_poll()
{
    if (!s_running)
        return;

    // A telnet szervert csak akkor indítjuk, ha kapcsolódtunk.
    if (WiFi.status() == WL_CONNECTED)
    {
        if (!s_serverUp)
        {
            s_server.begin();
            s_server.setNoDelay(true);
            s_serverUp = true;
        }
    }
    else
    {
        s_serverUp = false;
        return;
    }

    // Új kliens fogadása (egyszerre egy).
    if (s_server.hasClient())
    {
        if (!s_client || !s_client.connected())
        {
            if (s_client)
                s_client.stop();
            s_client = s_server.available();
            s_iac = 0;
            // char-mód: IAC WILL ECHO, IAC WILL SUPPRESS-GO-AHEAD
            const uint8_t neg[] = {255, 251, 1, 255, 251, 3};
            s_client.write(neg, sizeof(neg));
            s_client.print("== CYD serial terminal ==\r\n");
        }
        else
        {
            s_server.available().stop(); // 2. kliens elutasítása
        }
    }
}

int wifi_bridge_read(uint8_t *out, int max)
{
    if (!s_serverUp || !s_client || !s_client.connected())
        return 0;
    int n = 0;
    while (s_client.available() && n < max)
    {
        int b = s_client.read();
        if (b < 0)
            break;
        // Telnet IAC (0xFF) szekvenciák kiszurése.
        if (s_iac == 0)
        {
            if (b == 255) { s_iac = 1; continue; }
            out[n++] = (uint8_t)b;
        }
        else if (s_iac == 1)
        {
            if (b == 250) s_iac = 3;            // SB -> SE-ig
            else if (b >= 251 && b <= 254) s_iac = 2; // WILL/WONT/DO/DONT -> 1 opció
            else s_iac = 0;                     // egyéb parancs
        }
        else if (s_iac == 2)
        {
            s_iac = 0; // opció-byte elnyelve
        }
        else // s_iac == 3 (SB ... SE)
        {
            if (b == 240) s_iac = 0;
        }
    }
    return n;
}

void wifi_bridge_write(const uint8_t *data, int len)
{
    if (s_serverUp && s_client && s_client.connected())
        s_client.write(data, len);
}
