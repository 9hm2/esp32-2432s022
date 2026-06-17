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
    truecolor közelítés (`38;2;r;g;b`), **félkövér** (világos színként), **inverz**,
  - kurzor mentés/visszaállítás (`s`/`u`, `ESC 7`/`ESC 8`), láthatóság (`?25h/l`),
  - **alternatív képernyo** (`?1049`/`?47`/`?1047`) — `vim`/`nano`/`htop`/`less`
    teljes képernyos appokhoz (kilépéskor tiszta képernyo, a scrollback érintetlen),
  - **DEC vonalrajzoló karakterkészlet** (`ESC(0`) ASCII-közelítéssel (keretek),
  - **DSR/DA válaszok** (`ESC[6n`, `ESC[5n`, `ESC[c`) — a lekérdezo appoknak.
- **Egyedi LVGL rajzolás** kis monospace fonttal (elotér + háttér + inverz
  cellánként, blokk-kurzor).
- **Scrollback** (60 sor): **húzd le/fel a terminált** a régi sorok
  megtekintéséhez (scroll-lock: a nézet a helyén marad új adat érkezésekor is).
- **Wi-Fi telnet bevitel:** az ESP a megadott hálózathoz csatlakozik (STA), és
  egy **telnet szervert** (port 23) nyit. A telefonod/géped a **teljes
  billentyuzetérol** gépelsz (`nc <ip> 23`, PuTTY, telnet app), az ESP a Pi-nek
  továbbítja; a Pi kimenete a kijelzon ÉS a telnet kliensen is látszik.
  Bekapcsolás: **Beállítások (⚙) → Wi-Fi telnet**.
- **Képernyo-billentyuzet** és **Fn billentyu-panel** (nyilak/Ctrl stb.) szintén
  a **Beállítások** (⚙) menübol — vésztartaléknak, ha nincs Wi-Fi.
- **Beállítások** (⚙): **baud** (9600–230400), **sorvég** (nincs/LF/CRLF/CR),
  **helyi echo**, **tájolás** (álló/fekvo), **Wi-Fi telnet** — NVS-be mentve.
- A felso sáv jobb oldalán **⚙ beállítás** és **⟵ vissza**; Wi-Fi bekapcsolva a
  bal oldal **váltogatva** mutatja a **port-beállításokat** és az **IP-címet**.

## Wi-Fi telnet

A hálózati adatok a `src/wifi_bridge.cpp`-ben (`WIFI_SSID` / `WIFI_PASS`).
Bekapcsolás után a felso sáv kijelzi az IP-t; csatlakozz rá:

```bash
nc <ip> 23        # vagy: telnet <ip>, vagy PuTTY (raw/telnet, port 23)
```

A begépelt karakterek a Pi soros konzoljára mennek (a most használt RX/TX
bekötéssel), tehát teljes billentyuzetrol vezérelheted a Pi-t.

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

## Karaktersűrűség (kis kijelzo)

- A font egy **kis monospace** (DejaVu Sans Mono, 8 px, **bpp1**, szoros sor),
  cellaméret **5×8 px** — sok karakter fér ki. A fontot az `src/term_font.c`
  tartalmazza (generálva `lv_font_conv`-val).
- **Álló módban ~46 oszlop**, **fekvo módban ~62 oszlop**. A tájolás a
  **Beállítások** (⚙) menüben váltható, NVS-be mentve.

## Korlátok

- **Scrollback** 80 sor (efölött a legrégebbi sorok kiesnek).
- Truecolor (`38;2`) a 256-szín palettára **közelítve** jelenik meg.
- Néhány ritka szekvenciát (alternatív képernyo `?1049`, charset váltás) figyelmen
  kívül hagy; a tipikus konzol/`nano`/`htop`/`vim` kimenet helyesen jelenik meg.
- Max rács 64×40 cella.
