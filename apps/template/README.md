# App-sablon a grafikus SD-launcherhez

Kiindulási alap **saját appokhoz**, amiket a [launcher](../../firmware/bootloader/)
SD-kártyáról betölt és futtat (OTA flash-and-boot). Egy működő LVGL demót tartalmaz,
benne egy **„Vissza a launcherhez"** gombbal.

> Az `apps/` mappa tartalmazza a betölthető appokat: ez a `template/` a kiindulás,
> mellette pl. a `serial-terminal/` az első kész projekt.

## Miért ezt használd

- **Közös partíciótábla** a launcherrel (`../../firmware/bootloader/partitions.csv`)
  → az `ota_0` méret/offset és az adat-partíciók (nvs) egyeznek.
- Tartalmazza a **`return_to_launcher()`** helpert (`src/launcher_return.h`).
- A „ne jelöld valid-nak" konvenció miatt a **RESET is visszavisz** a menübe.

## Build és telepítés az SD-re

```bash
cd apps/template
pio run
# A betöltendő app-image:
#   .pio/build/app-template/firmware.bin
# Másold az SD-kártyára:
#   /apps/demo.bin   (tetszőleges nev, .bin kiterjesztessel)
```

> ⚠️ **A `firmware.bin`-t másold** (ez az app-image), NE a `bootloader.bin`-t vagy
> a `partitions.bin`-t, és NE valamilyen „merged/full flash" képet.

Ezután a launcherrel (a panelon) válaszd ki a listából — beflashel az `ota_0`-ba
és elindul.

## Visszatérés a launcherhez — kétféleképp (mindkettő működik)

1. **Kooperatív (gomb):** a demo „Vissza a launcherhez" gombja meghívja a
   `return_to_launcher()`-t → a `factory` partíciót állítja boot-célnak és
   újraindít.
2. **RESET (rollback):** mivel a launcher az appot „pending verify" állapotban
   indítja (rollback élesítve), és az app **nem** hívja a
   `esp_ota_mark_app_valid_cancel_rollback()`-ot, egy egyszerű **RESET** a
   bootloadert visszadobja a `factory`-ra (launcher).

### Mit NE csinálj az appban
- Ne hívd az `esp_ota_mark_app_valid_cancel_rollback()`-ot — különben a RESET-es
  visszatérés nem fog működni (az app „beragad").

## Saját app írása innen

A `src/main.cpp` `build_ui()`-ját cseréld a sajátodra; a grafikus init
(`smartdisplay_init()` + LVGL loop) maradhat. A `launcher_return.h`-t tartsd meg,
ha kell kooperatív vissza gomb. Az app mérete max. ~2.3 MB lehet (ota_0).
