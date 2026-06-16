# Grafikus SD-kártyás bootloader / app-indító — fejlesztési terv

> Állapot: **TERVEZÉS** (kód még nincs). Cél eszköz: **ESP32-2432S022C** (CYD 2.2",
> ESP32-WROOM-32, 4 MB flash, **nincs PSRAM**). Grafikus alap: a repó meglévő
> stackje (Arduino + LVGL 9 + esp32-smartdisplay).

## 1. Cél

Egy GUI-s „bootloader" (pontosabban **app-indító / launcher**) a panelra, amely:
- listázza az **SD-kártyán lévő kész `.bin` firmware-fájlokat**,
- a kiválasztottat **lefuttatja** az ESP32-n,
- érintőképernyős menüből kezelhető,
- visszatérési útat ad a menübe.

## 2. Alapvető műszaki korlát (ezt fontos tisztázni az elején)

Az ESP32 **nem tud `.bin`-t közvetlenül SD-kártyáról futtatni**:
- nincs „execute in place" (XIP) SD-ről — a kód flashből, MMU-mappinggel fut;
- a belső RAM (~320 KB) kevés egy teljes app RAM-ba töltéséhez;
- az app image flash-partícióhoz kötött (a 2nd-stage bootloader állítja be az MMU-t).

**Következmény:** „futtatás" = az app **belső flashbe írása, majd átindítás rá**.
Ez az **OTA „flash-and-boot"** minta. A kiválasztott app utána **natív sebességgel**
fut, mintha gyárilag flashelték volna.

### Mit jelent ez a `.bin`-ekre nézve
- A fájlnak egy **ESP32 alkalmazás-image**-nek kell lennie — az Arduino/ESP-IDF build
  **app `.bin`**-je (az, ami normál flasheléskor `0x10000`-re megy), **NEM** a
  „merged"/„full flash" `.bin` (ami bootloadert + partíciótáblát + appot tartalmaz).
- Az appnak **OTA-kompatibilisnek** kell lennie és **bele kell férnie** a launcher
  OTA partíciójába (lásd 4. pont).
- ⚠️ **Kompatibilitási megkötés:** a futtatott app a **launcher partíciótábláját**
  és **2nd-stage bootloaderét** használja. A kód lefut, de ha az app saját adat-
  partíciókat vár (SPIFFS/LittleFS/saját NVS-elrendezés), azok a launcher
  elrendezése szerint viselkednek. Tisztán számító/grafikus appoknál ez nem gond;
  saját fájlrendszert igénylőknél az elrendezésnek egyeznie kell.

## 3. Magas szintű architektúra

```
            ┌─────────────────────────────────────────┐
            │  2nd-stage bootloader (a launcher buildje) │
            └───────────────┬───────────────────────────┘
                            │ boot kiválasztás (otadata / rollback)
          ┌─────────────────┴───────────────────┐
          ▼                                       ▼
   ┌──────────────┐                       ┌──────────────────┐
   │  factory     │  ← LAUNCHER           │  ota_0           │ ← futtatott app
   │  (LVGL GUI,  │   (mindig ide esik    │  (ide flasheljük │
   │   SD, OTA)   │    vissza rollbacknél)│   az SD-ről)     │
   └──────────────┘                       └──────────────────┘
```

- **factory**: maga a launcher (GUI + SD + OTA logika).
- **ota_0**: ide írjuk és innen fut a kiválasztott app.
- **otadata/nvs/phy_init**: OTA állapot, beállítások.

## 4. Partíciótábla-terv (4 MB flash)

Javasolt `partitions.csv` (a pontos méretek a launcher és az appok igényéhez
hangolhatók):

```csv
# Name,   Type, SubType,  Offset,   Size
nvs,      data, nvs,      0x9000,   0x5000
otadata,  data, ota,      0xe000,   0x2000
phy_init, data, phy,      0x10000,  0x1000
factory,  app,  factory,  0x20000,  0x180000   # 1.5 MB  – launcher
ota_0,    app,  ota_0,    0x1A0000, 0x250000   # 2.31 MB – futtatott app
# (összesen ~0x3F0000, belefér 4 MB-ba)
```

- **App méretkorlát ≈ 2.3 MB** (ota_0). 4 MB-on ez a reális felső határ, ha a
  launcher LVGL-es.
- Csak **egy** app-slot kell (egyszerre egy app fut); a rollback-fallback a factory.

## 5. A futtatás folyamata (fő use-case)

1. **SD mount** — SPI busz: `CS=5, MOSI=23, SCLK=18, MISO=19` (a board szerint),
   FATFS-en.
2. **Lista** — a launcher beolvassa pl. a `/apps/*.bin` fájlokat (név, méret).
3. **Kiválasztás** (touch) → **validáció** flashelés előtt:
   - az image első byte-ja `0xE9` (ESP image magic),
   - méret ≤ ota_0 mérete,
   - (opcionálisan) chip-id / image header józanság-ellenőrzés.
4. **Flashelés**: `esp_ota_begin(ota_0)` → SD-ről **chunkokban** (`esp_ota_write`) →
   `esp_ota_end`. Közben **progress bar**.
5. **Boot beállítás**: `esp_ota_set_boot_partition(ota_0)` **pending** állapotban
   (nem confirmoljuk → rollback élesítve), majd `esp_restart()`.
6. Az ESP32 az **appba** bootol és fut.
7. **Vissza a menübe**: lásd 6. pont.

## 6. Visszatérés a launcherhez

**Választott megoldás — ESP-IDF rollback (módosítatlan appokkal is működik):**
- A launcher az appot **„pending verify"**-ként bootolja (sosem hívja a
  `esp_ota_mark_app_valid_cancel_rollback()`-ot az app helyett).
- Mivel az app (jellemzően) nem jelöli magát érvényesnek, a **következő
  RESET/újraindításkor a bootloader visszaesik a `factory`-ra** → előjön a menü.
- Felhasználói élmény: **app indítás → használat → RESET gomb → vissza a menübe.**
- Igényli: `CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE=y`.

**Alternatíva (kooperatív appok):** az app egy „Vissza" gombbal maga hívja a
`esp_ota_set_boot_partition(factory)` + `esp_restart()`-ot. Megbízhatóbb UX, de
app-oldali módosítást igényel. (A kettő kombinálható.)

## 7. GUI (LVGL) terv

- **Főképernyő**: görgethető **lista** a `.bin` fájlokról (név + méret), touch.
- **Elem-részletek / „Indítás"** gomb, megerősítéssel.
- **Flashelés-képernyő**: progress bar + státusz („Másolás… X/Y KB"), megszakítás.
- **Hibakezelés**: nincs SD / üres mappa / rossz image / túl nagy fájl → érthető
  üzenet.
- **Opcionális csiszolás**:
  - ikon az app mellé (azonos nevű `.png`/raw),
  - rendezés (név/dátum/méret), keresés,
  - app-metaadat egy `manifest.json`-ból (cím, verzió, leírás),
  - „Beállítások" (fényerő, alapértelmezett app, auto-boot időzítő).

## 8. Robusztusság / biztonság

- **Nem-téglásodó**: a `factory` (launcher) mindig sértetlen marad; megszakadt
  flashelésnél a rollback visszaviszi.
- **Image-ellenőrzés**: magic + méret a flashelés előtt; az `esp_ota_end` az
  image beágyazott **SHA-256**-ját is ellenőrzi.
- **Tápfeszültség**: flashelés alatt ne menjen el a táp. Akkus üzemnél figyelni
  (lásd a README power-bank/auto-lekapcsolás megjegyzéseit) — flashelés előtt
  érdemes „tartsd USB-n" figyelmeztetés.
- **Flash-kopás**: minden indítás újraírja az ota_0-t. Mérséklés: opcionális
  „már flashelve" gyorsítótár (az utoljára flashelt fájl SHA-jának NVS-be
  mentése → ha ugyanaz, ne írj újra, csak bootolj).

## 9. Javasolt projekt-/fájlstruktúra (a megvalósításhoz)

```
firmware/bootloader/                # külön PlatformIO env vagy külön projekt
├── platformio.ini                  # board=esp32-2432S022C, partíció CSV, rollback
├── partitions.csv                  # 4. pont
├── include/lv_conf.h               # a meglévő LVGL config (újrahasznosítva)
└── src/
    ├── main.cpp                    # init, LVGL loop
    ├── sd.cpp / sd.h              # SD mount + .bin listázás (FATFS)
    ├── ota_runner.cpp / .h        # esp_ota flash-and-boot + rollback
    └── ui/launcher_ui.cpp / .h    # LVGL lista, progress, hibák
```

(A jelenlegi `src/main.cpp` demó megmarad referenciának; a launcher külön env-be
kerülhet, hogy a két dolog ne keveredjen.)

## 10. Mérföldkövek (inkrementális fejlesztés)

| # | Mérföldkő | Tartalom | „Kész, ha…" |
|---|---|---|---|
| **M1** | Alapok | partíciótábla + factory launcher bebootol; SD mount; `.bin` lista a **soros porton** | listázza a fájlokat UART-on |
| **M2** | GUI lista | LVGL görgethető lista a `.bin`-ekről, touch-kiválasztás | a képernyőn látszik és válaszható a lista |
| **M3** | PoC flash-and-boot | egy **fix** `.bin` SD→ota_0 másolás + boot | egy ismert app elindul SD-ről |
| **M4** | Teljes folyamat | GUI-ból indítás + progress + hibakezelés | tetszőleges (kompatibilis) app indítható menüből |
| **M5** | Vissza a menübe | rollback élesítés, RESET → launcher | app után RESET visszahozza a menüt |
| **M6** | Csiszolás | ikonok, rendezés, manifest, beállítások, flash-cache | „termék" élmény |

## 11. Döntések (rögzítve) és még nyitott pontok

**Rögzített döntések:**
1. ✅ **A `.bin`-ek forrása: saját, erre a launcherre buildelt appok** (OTA app
   `.bin`, a 4. pont partíció-elrendezésével). Ez a legmegbízhatóbb: a kód és az
   adat-partíciók is passzolnak. → Lásd a 11/a pontot (app-sablon).
2. ✅ **Visszatérés a menübe: MINDKÉT mód.** Alapból a **rollback + RESET**
   (akkor is működik, ha az app nem tartalmaz semmit), és emellett **kooperatív
   „Vissza a launcherbe" helper** is elérhető, amit az app beépíthet egy gombra.

**Még nyitott (a megvalósításkor eldöntendő):**
3. **App méretkorlát ~2.3 MB** elfogadható? (4 MB flash miatt ez a határ.)
4. **Flash-cache** kell-e (ne flasheljen újra azonos appot SHA alapján), vagy
   mindig SD-ről?
5. **App-adatok**: kell-e közös NVS/SPIFFS-terület az appoknak (a partíciótáblában
   előre lefoglalva), vagy tisztán önállóak?
6. **Külön projekt vs. külön PlatformIO env** a meglévő repón belül a launcherhez.

### 11/a. App-sablon (mert saját appokat buildelünk) — tervezett deliverable

Mivel a `.bin`-eket magunk fordítjuk, érdemes egy **közös app-sablont** adni,
hogy minden app biztosan kompatibilis legyen a launcherrel:

- **Közös `partitions.csv`** (a 4. pontból) — minden app *ugyanazzal* a
  partíciótáblával épüljön, hogy az ota_0 offset/méret és az esetleges közös
  adat-partíciók egyezzenek.
- **`platformio.ini` szelet** az appokhoz: `board = esp32-2432S022C`, ugyanaz a
  partíciótábla, rollback engedélyezve.
- **„Vissza a launcherbe" helper** (a kooperatív módhoz), pl.:

  ```cpp
  // visszaer a launcherhez: a factory-t allitja boot-partiicionak, majd reset
  #include <esp_ota_ops.h>
  void return_to_launcher() {
      const esp_partition_t *factory = esp_partition_find_first(
          ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, NULL);
      if (factory) esp_ota_set_boot_partition(factory);
      esp_restart();
  }
  ```

- **(Opcionális) önérvényesítés tiltása**: az app NE hívja a
  `esp_ota_mark_app_valid_cancel_rollback()`-ot, hogy a RESET-es rollback is
  működjön. (Arduino appoknál ez alapból így van.)
- A sablon a repóban pl. `firmware/app-template/` alatt élhet.

## 12. Kockázatok

- **Flash szűkösség** (launcher + 1 app 4 MB-on) — a launchert karcsún kell tartani
  (LVGL feature-ek nyesegetése).
- **App-kompatibilitás**: eltérő partíciótáblát/IDF-verziót feltételező appok
  adat-partíciói nem fognak passzolni (a kód fut, az adat lehet, hogy nem).
- **Akkus flashelés**: tápkimaradás flashelés közben (rollback véd, de bosszantó).
- **„Merged” bin tévedés**: ha valaki a full-flash `.bin`-t teszi SD-re az app-`.bin`
  helyett, az nem fog jól bootolni → erős validáció + dokumentáció kell.

---

### Következő lépés
A 11. pont döntései után az **M1** elindítható (partíciótábla + SD-listázás).
A grafikus alap (LVGL + smartdisplay + touch) már működik és újrahasznosítható.
