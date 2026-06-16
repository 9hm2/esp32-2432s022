# Serial Terminal — Raspberry Pi soros konzol a CYD kijelzon

Az [SD-launcherrel](../../firmware/bootloader/) betöltheto app: **valódi, látható
soros terminál**. Az UART-on érkezo szöveget görgetheto terminálablakban mutatja,
képernyo-billentyuzetrol parancsot küldhetsz, és a beállítások perzisztensek.

## Funkciók

- **Terminál-kimenet** monospace (UNSCII) fonttal, auto-görgetéssel.
- Bejövo adat **ANSI/VT100 escape-szekvenciák szurésével** (tiszta, olvasható kép),
  `CR/LF`, `TAB`, `Backspace` kezeléssel.
- **Képernyo-billentyuzet** a parancsok küldéséhez (⌨ gomb).
- **Beállítások** (⚙): **baud** (9600–230400), **sorvég** (nincs/LF/CRLF/CR),
  **helyi echo** — NVS-be mentve (újraindítás után is megmarad).
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

## Karaktersűrűség (kis kijelzo)

- A font **UNSCII 8×8** — az LVGL-be épített **legkisebb monospace** font.
- Térközök nullázva, minimális padding → **álló módban ~29 oszlop × ~30 sor**
  fér ki a 240×254 px-es terminálablakban.
- **Fekvo módban (landscape) ~40 oszlop × ~22 sor** lenne (szélesebb sorok). Ha
  ezt szeretnéd, a `lv_display_set_rotation(...LANDSCAPE...)` + a UI méretek
  átállítása kell — szólj, és átkapcsolom (vagy beállítássá teszem).

## Korlátok

- Nem teljes VT100-emuláció: az escape-szekvenciákat **eldobja** (nem értelmezi a
  kurzormozgást/színt), de a szöveg tisztán olvasható marad.
- A kimeneti puffer kb. 4 KB (a régi sorok kigörögnek).
