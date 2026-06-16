// Wi-Fi (STA) + telnet szerver: a meglévo hálózathoz csatlakozik, és egy telnet
// kliens (telefon/PC) billentyuzetét továbbítja a Pi felé (UART0), a Pi kimenetét
// pedig a kliensnek. Így a kis érinto-billentyuzet helyett teljes billentyuzetrol
// gépelhetsz a hálózaton át.
#pragma once

#include <Arduino.h>

void wifi_bridge_start();      // STA csatlakozás indítása (+ telnet, ha kapcsolódott)
void wifi_bridge_stop();
bool wifi_bridge_running();    // be van-e kapcsolva a Wi-Fi funkció
bool wifi_bridge_connected();  // csatlakozott-e a hálózathoz
String wifi_bridge_ip();       // aktuális IP (vagy üres)

// A telnet kliens felol érkezett byte-ok (a Pi-nek továbbítandó). IAC-szurt.
int wifi_bridge_read(uint8_t *out, int maxlen);
// A Pi kimenetének tükrözése a telnet kliensnek.
void wifi_bridge_write(const uint8_t *data, int len);

void wifi_bridge_poll();       // a fo loopból: kapcsolat + kliens kezelése
