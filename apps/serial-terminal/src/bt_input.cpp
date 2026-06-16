#include "bt_input.h"

#include <NimBLEDevice.h>
#include <esp_random.h>

// Nordic UART Service (NUS) UUID-k — a legtöbb BLE terminál app ezt használja.
#define NUS_SERVICE "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define NUS_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define NUS_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// SPSC gyuru-puffer a BLE-rol érkezo byte-okhoz (BLE task -> loop task).
static constexpr int RXSZ = 1024;
static uint8_t s_rx[RXSZ];
static volatile uint16_t s_head = 0, s_tail = 0;

static volatile bool s_connected = false;
static volatile uint32_t s_passkey = 0;
static volatile bool s_pairing = false;
static volatile bool s_authPending = false;
static volatile bool s_authOk = false;

static void pushByte(uint8_t b)
{
    uint16_t n = (s_head + 1) % RXSZ;
    if (n != s_tail)
    {
        s_rx[s_head] = b;
        s_head = n;
    }
}

class RxCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic *c) override
    {
        NimBLEAttValue v = c->getValue();
        const uint8_t *d = v.data();
        size_t n = v.length();
        for (size_t i = 0; i < n; i++)
            pushByte(d[i]);
    }
};

class ServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer *, ble_gap_conn_desc *) override { s_connected = true; }

    void onDisconnect(NimBLEServer *) override
    {
        s_connected = false;
        s_pairing = false;
        NimBLEDevice::startAdvertising(); // újra hirdetünk
    }

    // DISPLAY_ONLY: mi generáljuk és jelenítjük meg a kódot, a peer beírja.
    uint32_t onPassKeyRequest() override
    {
        uint32_t pk = (esp_random() % 900000) + 100000; // 6 jegyu
        s_passkey = pk;
        s_pairing = true;
        return pk;
    }

    void onAuthenticationComplete(ble_gap_conn_desc *desc) override
    {
        s_authOk = desc->sec_state.encrypted;
        s_authPending = true;
        s_pairing = false;
        s_passkey = 0;
    }
};

static RxCallbacks s_rxCb;
static ServerCallbacks s_srvCb;

void bt_init(const char *name)
{
    NimBLEDevice::init(name);
    NimBLEDevice::setSecurityAuth(true, true, true); // bonding, MITM, secure conn
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_ONLY);
    // Kulcsdisztribúció: az ENC (LTK) és ID (IRK) kulcsokat mindkét irányba
    // kicseréljük — e nélkül a bond NEM tárolódik, és az újracsatlakozás (a már
    // párosítottnak hitt eszközrol) meghiúsul / a kapcsolat bontásra kerül.
    NimBLEDevice::setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);
    NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);

    NimBLEServer *srv = NimBLEDevice::createServer();
    srv->setCallbacks(&s_srvCb);

    NimBLEService *svc = srv->createService(NUS_SERVICE);
    NimBLECharacteristic *rx = svc->createCharacteristic(
        NUS_RX, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR);
    rx->setCallbacks(&s_rxCb);
    svc->createCharacteristic(NUS_TX, NIMBLE_PROPERTY::NOTIFY);
    svc->start();

    NimBLEAdvertising *adv = NimBLEDevice::getAdvertising();
    adv->addServiceUUID(NUS_SERVICE);
    adv->setScanResponse(true);
    NimBLEDevice::startAdvertising();
}

int bt_read(uint8_t *out, int max)
{
    int c = 0;
    while (s_tail != s_head && c < max)
    {
        out[c++] = s_rx[s_tail];
        s_tail = (s_tail + 1) % RXSZ;
    }
    return c;
}

bool bt_connected() { return s_connected; }
int bt_bond_count() { return NimBLEDevice::getNumBonds(); }
uint32_t bt_passkey() { return s_passkey; }
bool bt_pairing() { return s_pairing; }

bool bt_take_auth_result(bool *ok)
{
    if (s_authPending)
    {
        *ok = s_authOk;
        s_authPending = false;
        return true;
    }
    return false;
}

void bt_unpair_all() { NimBLEDevice::deleteAllBonds(); }

void bt_repair()
{
    NimBLEDevice::deleteAllBonds();
    if (!NimBLEDevice::getAdvertising()->isAdvertising())
        NimBLEDevice::startAdvertising();
}
