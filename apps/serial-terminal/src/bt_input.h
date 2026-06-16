// Bluetooth (BLE) bemenet a terminálhoz — NimBLE alapú Nordic UART Service.
// Egy telefon/PC csatlakozik, és a NUS RX karakterisztikába írt byte-ok a
// terminálba (és a Pi felé) kerülnek. Párosítás passkey-megjelenítéssel és
// bonding-gal (a párosítás megjegyzése NVS-ben).
#pragma once

#include <Arduino.h>

// Inicializálás + hirdetés indítása az adott eszköznévvel.
void bt_init(const char *name);

// A BLE-n érkezett byte-ok kiolvasása (a fo loop hívja). Visszaadja a darabszámot.
int bt_read(uint8_t *out, int maxlen);

bool bt_connected();
int bt_bond_count();             // párosított (bonded) eszközök száma
uint32_t bt_passkey();           // épp megjelenítendo 6 jegyu kód (0 = nincs)
bool bt_pairing();               // folyamatban van-e párosítás

// Ha volt új hitelesítési eredmény, true-t ad és *ok-ba írja a sikerességet.
bool bt_take_auth_result(bool *ok);

void bt_unpair_all(); // minden párosított eszköz törlése
void bt_repair();     // újrapárosítás: bondok törlése + hirdetés
