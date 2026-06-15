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
| USB | CH340 USB-UART híd, micro-USB |
| Tápfeszültség | 5 V (USB) |

> **Variánsok:** `…S022C` = kapacitív touch (CST816S), `…S022N` = touch nélkül.
> Ez a repó a **C (touch-os)** változatra van konfigurálva.

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

### Szabad / kivezetett GPIO-k
A board oldalsó csatlakozóin elérhető szabad lábak (a konkrét silk-screen a
panel revíziójától függ): jellemzően **GPIO35** (csak bemenet), **GPIO34**
(csak bemenet), **GPIO0** (BL-lel megosztva), valamint a JST P3/CN1 portokon
kivezetett I2C (21/22). Bővítés előtt ellenőrizd, hogy a láb nem ütközik-e a
kijelző buszával.

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
