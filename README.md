# ESP32-2432S022C — CYD 2.2" fejlesztői keretrendszer

Teljes fejlesztői keretrendszer a **Sunton ESP32-2432S022C** ("Cheap Yellow
Display", CYD 2.2") panelhez, grafikus stack-kel együtt: **PlatformIO + Arduino +
LVGL 9 + esp32-smartdisplay**.

A repó tartalmaz egy működő grafikus demót (címke, érintésre reagáló gomb,
háttérvilágítás csúszka, állapotsor), valamint a board teljes hardveres
dokumentációját.

---

## 1. Az eszköz — műszaki összefoglaló

| Jellemző | Érték |
|---|---|
| Gyártó / típus | Sunton **ESP32-2432S022C** |
| MCU | **ESP32-WROOM-32**, dual-core Xtensa LX6 @ 240 MHz |
| Flash | 4 MB (DIO @ 40 MHz) |
| RAM | 520 KB SRAM, **PSRAM nincs** |
| Vezeték nélküli | Wi-Fi b/g/n + Bluetooth/BLE |
| Kijelző | **2.2" IPS TFT, 240×320**, **ST7789** vezérlő |
| Kijelző busz | **8 bites párhuzamos i80 (8080)** — *nem* SPI! |
| Érintő | **CST816S** kapacitív, I2C (a „C" variáns) |
| Tároló | microSD foglalat (SPI) |
| Hang | beépített hangszóró-kimenet (GPIO26) |
| USB | CH340 USB-UART híd, **USB Type-C** |
| Tápfeszültség | 5 V (USB Type-C), kb. 100 mA |
| Akkumulátor | 1 cellás Li-ion/LiPo csatlakozó **töltéssel + védelemmel** (lásd 2. szakasz) |

> **Variánsok:** `…S022C` = kapacitív touch (CST816S), `…S022N` = touch nélkül.
> Ez a repó a **C (touch-os)** változatra van konfigurálva.

> **Márkajelzés (GUITION):** ezt a panelt több gyártó/forgalmazó is árulja —
> Sunton (`ESP32-2432S022C`), **Guition** (silkscreen jellemzően `JC2432W022C`),
> DIYmalls stb. Ezek **elektromosan ugyanaz a kialakítás**, ezért a
> `board = esp32-2432S022C` definíció és az itteni kód **mindegyikre jó**,
> kódmódosítás nélkül. Egy ismert apró revíziós különbség: néhány **GUITION**-jelű
> darabon a külső I2C/bővítő csatlakozó (**CN1**) 3-as lába **nincs bekötve**
> a GPIO22-re (SCL) — emiatt csak a *külső* I2C-eszközöket érintheti, a fedélzeti
> CST816S touch a belső buszon ettől függetlenül működik.

### Fontos eltérés a „klasszikus" CYD-től (2432S028R)
A 2.8"-os CYD (`2432S028R`) ILI9341 kijelzőt **SPI**-n és XPT2046 **rezisztív**
touch-ot használ. A **2.2"-os 022C ezzel szemben**:
- **ST7789** vezérlő **8 bites párhuzamos (i80) buszon** — sokkal gyorsabb, de
  jóval több GPIO-t foglal, és a `TFT_eSPI` SPI-alapú konfigurációk **nem**
  működnek rá közvetlenül.
- **CST816S kapacitív** touch I2C-n.

Emiatt a board-hoz az **ESP-IDF `esp_lcd` i80 driver** (amit az
esp32-smartdisplay csomagol) a legmegbízhatóbb út.

---

## 2. GPIO kiosztás (mérvadó)

A board-definíció: [`boards/esp32-2432S022C.json`](boards/esp32-2432S022C.json)
(forrás: `rzeldent/platformio-espressif32-sunton`).

### Kijelző — ST7789, 8 bites i80 párhuzamos busz
| Jel | GPIO | Megjegyzés |
|---|---|---|
| WR (write strobe) | **4** | |
| RD (read strobe) | **2** | |
| DC (data/command) | **16** | |
| CS (chip select) | **17** | |
| RST (reset) | **NC** | nincs bekötve (szoftveres reset) |
| Háttérvilágítás (BL) | **0** | PWM-mel szabályozható |
| Adat D0 | **15** | |
| Adat D1 | **13** | |
| Adat D2 | **12** | |
| Adat D3 | **14** | |
| Adat D4 | **27** | |
| Adat D5 | **25** | |
| Adat D6 | **33** | |
| Adat D7 | **32** | |

Pixel órajel: 12 MHz. Színmélység: 16 bit (RGB565).

### Érintő — CST816S, I2C
| Jel | GPIO | Megjegyzés |
|---|---|---|
| SDA | **21** | belső felhúzás engedélyezve |
| SCL | **22** | belső felhúzás engedélyezve |
| INT | **NC** | nincs bekötve (pollozott) |
| RST | **NC** | nincs bekötve |
| I2C cím | **0x15** | 400 kHz |

### microSD — SPI
| Jel | GPIO |
|---|---|
| CS | **5** |
| MOSI | **23** |
| SCLK | **18** |
| MISO | **19** |

### Egyéb
| Periféria | GPIO |
|---|---|
| Hangszóró kimenet | **26** |

### Fizikai csatlakozók és tápellátás
A hivatalos Sunton (Shenzhen Jingcai) termékspecifikáció és a panel fotója
alapján a következő csatlakozók/kezelőszervek vannak a kártyán:

| Jelölés a panelen | Típus | Funkció |
|---|---|---|
| **type-C** | USB Type-C | Tápellátás (5 V) + programozás (CH340 USB-UART) + akkutöltés |
| **Battery interface** | JST 1.25 mm **2 tűs** | **Akkumulátor-csatlakozó** (1 cellás Li-ion/LiPo, BAT+/GND) |
| **Battery button switch** | kapcsoló | Az **akku tápjának** be-/kikapcsolása |
| **4P 1.25 Power supply base** | JST 1.25 mm **4 tűs** (HC-1.25-4PWT) | Táp + soros port: **Vin(5V) / GND / U0TXD(GPIO1) / U0RXD(GPIO3)** — soros vonalak 100 Ω-on át (rajz szerint) |
| **Speak** | JST 1.25 mm **2 tűs** | Hangszóró (GPIO26-on keresztül, audio erősítővel) |
| **TF** | microSD foglalat | SPI (CS=5, MOSI=23, SCLK=18, MISO=19) |
| **RESET / BOOT** | nyomógomb | Újraindítás / bootloader mód (egykattintásos feltöltés) |

**Az akkumulátorról — a gyártói specifikáció szerint (korrigált infó):**
- Van **2 tűs akkucsatlakozó** (a panelen „Battery interface") és egy hozzá
  tartozó **kapcsoló** („Battery button switch"). Ez fizikai tápbemenet, **nem
  GPIO** — ezért nem szerepel a board-definíció GPIO-listájában.
- **A panel TUD tölteni — a kapcsolási rajz is megerősíti.** A spec szerint
  *„Supports lithium battery power supply, supports both charging and discharging,
  and has over charging and over discharging protection."* A rajzon ezt egy
  **integrált power-bank IC** (U8) valósítja meg: bemenet 5 V (USB-C), `SW` lábán
  egy **L1 induktor** boost-ot ad, kimenete a **VOUT-BAT** rendszerszint, `BAT`
  lábán a cella, plusz `KEY` (a „Battery button switch"), `LED1..LED3`
  töltésjelző kimenetek és túltöltés/mélykisütés-védelem. A topológia (VIN / KEY /
  LED1-3 / BAT / VOUT / SW+induktor) egy **IP5306-osztályú** power-bank IC-re vall.
- **Tápút (rajz szerint):** USB-C 5 V → `D1` (1N5819 Schottky) + `Q1` (AO3401
  P-MOSFET, fordított-polaritás/ideal-diode) → power-bank IC (U8) → **VOUT-BAT** →
  **két külön AMS1117-3.3 LDO**: `U7` adja a **3.3 V-ot az ESP32-nek**, `U1` a
  **3.3 V-ot a TFT-nek**. A VOUT-BAT-ot az IC akár USB-ről, akár az akkuról
  állítja elő → USB nélkül, akkuról is megy a panel.
- ⚠️ **Nincs akkufeszültség-mérés az ESP32 felé — ezt a rajz is megerősíti:**
  nincs feszültségosztó a `BAT`-ról egyetlen ADC GPIO-ra sem. A töltöttséget az IC
  saját `LED1..LED3` kimenetei jelzik, ezek nem mennek a vezérlőre. Ha szoftveres
  töltöttség-kijelzést akarsz, magadnak kell egy feszültségosztót egy szabad ADC1
  lábra (pl. GPIO35/GPIO34, csak bemenet) kötnöd.
- A pontos IC-cikkszám a rajzon nem olvasható ki egyértelműen, revíziónként
  eltérhet — nagy töltőáram/extra terhelés előtt érdemes a saját paneleden
  ellenőrizni.

### Szabad / kivezetett GPIO-k
A board oldalsó csatlakozóin elérhető szabad lábak (a konkrét silk-screen a
panel revíziójától függ): jellemzően **GPIO35** (csak bemenet), **GPIO34**
(csak bemenet), **GPIO0** (BL-lel megosztva), valamint a JST P3/CN1 portokon
kivezetett I2C (21/22). Bővítés előtt ellenőrizd, hogy a láb nem ütközik-e a
kijelző buszával. Akkufeszültség-mérésre a GPIO34/35 (ADC1) a jó választás,
mert ezek Wi-Fi mellett is működnek.

---

## 3. Projektstruktúra

```
.
├── platformio.ini                 # PlatformIO konfiguráció (env: esp32-2432S022C)
├── boards/
│   └── esp32-2432S022C.json       # board-definíció (pinout build flag-ekkel)
├── include/
│   └── lv_conf.h                  # LVGL 9 konfiguráció
├── src/
│   └── main.cpp                   # grafikus + touch demó
├── .gitignore
└── README.md
```

A board-definíciót szándékosan **közvetlenül a repóba** tettük (nem git
submodule), így a projekt önállóan, hálózati submodule nélkül is fordul. A
`platformio.ini`-ben a `boards_dir = boards` sor miatt a PlatformIO automatikusan
felismeri a `esp32-2432S022C` board-ot.

---

## 4. Build és feltöltés

### Előfeltételek
- [PlatformIO Core](https://platformio.org/install/cli) (`pio`) vagy a VS Code
  PlatformIO bővítmény.

### Parancsok
```bash
# Fordítás
pio run

# Fordítás + feltöltés a csatlakoztatott board-ra
pio run --target upload

# Soros monitor (115200 baud)
pio device monitor
```

Első buildkor a PlatformIO letölti az `espressif32` platformot, az
`esp32_smartdisplay` libet és az LVGL 9-et. A háttérvilágítás induláskor 50%-on
van; a demó csúszkájával állítható.

---

## 5. A grafikus stack működése

1. **`smartdisplay_init()`** felhúzza az `esp_lcd` i80 buszt (ST7789), bekapcsolja
   a háttérvilágítást és regisztrálja a CST816S touch-ot LVGL input device-ként.
2. Az **LVGL** a `lv_conf.h` szerint dolgozik: 16 bites szín, belső heap
   (`LV_MEM_SIZE = 48 KB`), montserrat fontok.
3. A `loop()` minden ciklusban frissíti az LVGL tick-et (`lv_tick_inc`) és
   meghívja a `lv_timer_handler()`-t, ami rajzol és kezeli az érintést.

A rajzbuffer a board-definícióban `DISPLAY_WIDTH*DISPLAY_HEIGHT/8` pixel, a belső
(DMA-képes) RAM-ban — mivel a panelen **nincs PSRAM**.

### Saját UI írása
A `src/main.cpp` `build_ui()` függvénye a kiindulópont. Onnantól tiszta LVGL 9
API-t használsz (`lv_label_create`, `lv_button_create`, `lv_slider_create`,
`lv_obj_add_event_cb`, …). A beépített demók is kipróbálhatók:
```cpp
lv_demo_widgets();   // a lv_conf.h-ban engedélyezve (LV_USE_DEMO_WIDGETS)
```

---

## 6. Buktatók / tippek

- **Ez nem SPI kijelző.** A `TFT_eSPI`-hez írt CYD példák (ILI9341/SPI) nem
  működnek rá átalakítás nélkül. Maradj az i80 driverrel (smartdisplay).
- **RST és INT lábak NC-k.** A driver konfigurációban `GPIO_NUM_NC`-ként
  szerepelnek; ne próbálj rájuk valós GPIO-t kötni a kódban.
- **Nincs PSRAM.** A nagy LVGL bufferek/animációk korlátosak; tartsd a
  `LV_MEM_SIZE`-t és a rajzbuffert mértékkel.
- **Tükrözés/forgatás.** Ha fordítva jelenne meg a kép vagy a touch, állítsd a
  `DISPLAY_MIRROR_*` / `TOUCH_MIRROR_*` / `*_SWAP_XY` flag-eket a board JSON-ban,
  vagy futásidőben `lv_display_set_rotation()`-nel.
- **GPIO0 (BL).** A háttérvilágítás a boot-strap GPIO0-ra van kötve; feltöltéskor
  ez ritkán okozhat fennakadást — ha gond van, tartsd nyomva a BOOT gombot.

---

## 7. Hivatkozások

- esp32-smartdisplay (LVGL driver lib): <https://github.com/rzeldent/esp32-smartdisplay>
- Board-definíciók: <https://github.com/rzeldent/platformio-espressif32-sunton>
- 2432S022C működés bejelentés: <https://github.com/rzeldent/esp32-smartdisplay/discussions/128>
- LVGL dokumentáció: <https://docs.lvgl.io/>
- CYD általános referencia (028R): <https://randomnerdtutorials.com/cheap-yellow-display-esp32-2432s028r/>
- **Hivatalos gyártói specifikáció** (Shenzhen Jingcai, ESP32-2432S022N/C — akku/töltés, csatlakozók): <https://make.net.za/wp-content/datasheets/Shenzhen%20Jingcai%20Intelligent%20ESP32-2432S022%20Product%20Spec.pdf>
- A tápellátás/USB/SD szakaszt a panel **hivatalos kapcsolási rajza** alapján is ellenőriztük (POWER / USB / Lithium battery charging / SD_Card blokkok).
- DIYmalls 2432S022C felhasználói kézikönyv (csatlakozók, akku/P1/P2/P3): <https://manuals.plus/asin/B0DH1P13DW>
- Sunton hivatalos 2432S022 kódbázis: <https://github.com/lsdlsd88/2.2inch_ESP32-2432S022>
