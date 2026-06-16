# Serial Terminal — Raspberry Pi soros konzol a CYD kijelzon

Az [SD-launcherrel](../../) betöltheto app: **valódi, látható
soros terminál**. Az UART-on érkezo szöveget görgetheto terminálablakban mutatja,
képernyo-billentyuzetrol parancsot küldhetsz, és a beállítások perzisztensek.

## Funkciók

- **Valódi VT100/ANSI terminál** (nem csak teletype): karakterrács + kurzor +
  escape-szekvenciák. Támogatott:
  - kurzormozgás (CUU/CUD/CUF/CUB, CUP/HVP, CHA, VPA, NEL, IND/RI),
  - törlés (ED `J`, EL `K`), sor/karakter beszúrás-törlés (`L M P @ X`),
  - görgetés és **görgetési régió** (DECSTBM `r`, `S`/`T`),
  - **SGR színek/attribútumok**: 16 alap-, **256-szín** (`38;5;n`/`48;5;n`),
    truecolor közelítés (`38;2;r;g;b`), **félkövér**, **inverz**, alaphelyzet,
  - kurzor mentés/visszaállítás (`s`/`u`), kurzor láthatóság (`?25h/l`).
- **Egyedi LVGL rajzolás** monospace **UNSCII 8×8** fonttal (elotér + háttér +
  inverz cellánként, blokk-kurzor).
- **Scrollback** (80 sor): **húzd le/fel a terminált** a régi sorok
  megtekintéséhez (scroll-lock: a nézet a helyén marad új adat érkezésekor is).
- **Képernyo-billentyuzet** a parancsok küldéséhez (sorvégződéssel) — a
  **Beállítások** (⚙) menüben a „Billentyuzet" gombbal hívható elo; alapból nem
  jelenik meg (több hely a terminálnak).
- **Fn billentyu-panel**: nyers byte-ok közvetlenül a Pi-nek —
  **Esc, Tab, Ctrl-C/D/Z, nyilak (↑↓←→), Backspace, Enter** (interaktív
  programokhoz: `nano`, `vim`, `htop`, `top`, shell job control). A
  **Beállítások** (⚙) menüből hívható elo.
- **Bluetooth (BLE) bemenet:** telefon/PC csatlakozhat, és a beírt karakterek a
  terminálba (a Pi felé) kerülnek. Párosítás **6 jegyu kód generálással +
  kijelzéssel**, **siker-visszajelzéssel**, **bonddal** (megjegyzi a párosítást),
  **újrapárosítással** és **párosítás-törléssel** — a **Beállítások → Bluetooth**
  menüben. Részletek lentebb.
- **Beállítások** (⚙): **baud** (9600–230400), **sorvég** (nincs/LF/CRLF/CR),
  **helyi echo**, **tájolás** (álló/fekvo) — NVS-be mentve.
- **Törlés** (🗑) és **Vissza a launcherhez** (⟵).

## Bekötés a Raspberry Pi-hez (P1 port)

A panel **P1** („4P 1.25 Power supply base") portja az **UART0**-t vezeti ki:

| Pi (40-pin) | Board (P1) |
|---|---|
| GPIO14 TXD (pin 8) | **RXD = GPIO3** |
| GPIO15 RXD (pin 10) | **TXD = GPIO1** |
| GND | **GND** |
| 5V (opcionális táp) | **Vin (5V)** |

- A jelszint **3.3 V** (kompatibilis a Pi-vel). **5V-ot ne köss** a TX/RX-re.
- A Pi oldalán engedélyezd a soros konzolt (`raspi-config` → Interface → Serial),
  vagy köss rá bármilyen soros eszközre.
- Az app **nem** ír debug üzenetet az UART0-ra, hogy ne zavarja a kapcsolatot.

> Megjegyzés: az UART0 egyben a programozó/USB konzol is. A terminálhasználathoz a
> Pi-t a P1-re kösd; a panelt táplálhatod a P1 Vin-rol vagy USB-rol.

## Build és telepítés

```bash
cd apps/serial-terminal
pio run
# app-image: .pio/build/serial-terminal/firmware.bin  ->  SD: /apps/serial.bin
```

Majd a panelon a launcherbol válaszd ki — beflashel az `ota_0`-ba és elindul.
RESET vagy a ⟵ gomb visszavisz a launcherbe.

## Bluetooth (BLE) bemenet

A panel BLE-perifériaként hirdeti magát **`CYD-Terminal`** néven, **Nordic UART
Service (NUS)** karakterisztikával. Bármely BLE soros terminál app (pl. *nRF
Connect*, *Serial Bluetooth Terminal* BLE módban) csatlakozhat, és a beírt
karakterek a terminálba (a Pi felé) mennek — mintha a képernyo-billentyuzeten
gépelnél. A BT-n bevitt **nyomtatható karakterek a CYD kijelzon is megjelennek**
(helyi echo; az ESC/vezérlo-szekvenciák csak a Pi felé mennek, hogy ne rontsák a
helyi képet).

### Párosítás
1. A telefonon csatlakozz a `CYD-Terminal`-hoz.
2. A panel **6 jegyu kódot generál és kijelez** (felugró ablak).
3. Írd be ezt a kódot a telefonon. Siker esetén **„Sikeres parositas!"** üzenet.
4. A párosítás **megjegyzodik** (bond, NVS-ben) — legközelebb automatikusan
   visszacsatlakozik.

### Kezelés — Beállítások → Bluetooth
- **Allapot**: hirdetés / kapcsolódva + a párosított eszközök száma.
- **Ujraparositas**: a meglévo bondok törlése és új párosítás engedélyezése.
- **Parositas torlese**: az összes párosított eszköz törlése.

> Biztonság: **bonding + MITM + Secure Connections**, *DisplayOnly* I/O — a kódot
> a panel generálja és mutatja, a társeszköz írja be. A bond kulcsai (ENC+ID)
> mindkét irányba cserélodnek, így az **újracsatlakozás** is muködik.

> ⚠️ Ha korábbi (hibás) build-del már párosítottál, a telefonon **felejtsd el az
> eszközt** ("Forget"), a panelon pedig **Beállítások → Bluetooth → Clear
> pairing**, majd párosíts újra — különben az elavult bond miatt nem fog
> visszacsatlakozni.

## Karaktersűrűség (kis kijelzo)

- A font egy **kis monospace** (DejaVu Sans Mono, 9 px, **bpp1** — gyors,
  alfakeverés nélkül), cellaméret **6×10 px** — keskenyebb az UNSCII 8×8-nál.
  A fontot az `src/term_font.c` tartalmazza (generálva `lv_font_conv`-val).
- **Álló módban ~39 oszlop × ~25 sor**, kis margóval (a szélso oszlop sem lóg le).
- **Fekvo módban ~52 oszlop × ~21 sor**. A tájolás a **Beállítások** (⚙)
  menüben váltható, NVS-be mentve.

## Korlátok

- **Scrollback** 80 sor (efölött a legrégebbi sorok kiesnek).
- Truecolor (`38;2`) a 256-szín palettára **közelítve** jelenik meg.
- Néhány ritka szekvenciát (alternatív képernyo `?1049`, charset váltás) figyelmen
  kívül hagy; a tipikus konzol/`nano`/`htop`/`vim` kimenet helyesen jelenik meg.
- Max rács 60×40 cella (a tájolásból adódó méret bõven belefér).
