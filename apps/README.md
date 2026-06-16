# apps/ — a launcherrel betöltheto alkalmazások

Ezek a projektek a [grafikus SD-launcherrel](../) (a repó gyökere) töltodnek be
és futnak (OTA flash-and-boot). Mindegyik önálló PlatformIO projekt, közös
partíciótáblával (`partitions.csv` (gyökér)).

| Mappa | Leírás |
|---|---|
| [`template/`](template/) | Kiindulási **app-sablon** (LVGL demó + „Vissza a launcherhez" helper) |
| [`serial-terminal/`](serial-terminal/) | **Soros terminál** a Raspberry Pi soros portjához (beállítható baud/sorvég/echo, képernyo-billentyuzet) |

## Új app készítése

1. Másold a `template/` mappát egy új névre az `apps/` alatt.
2. Írd át a `src/main.cpp`-t (a grafikus init maradhat).
3. Tartsd meg a `launcher_return.h`-t a „Vissza" gombhoz.
4. `pio run` → a `.pio/build/<env>/firmware.bin` az SD `/apps/` mappájába.

## Telepítés az SD-kártyára

Minden app a saját `firmware.bin`-jét adja (ez az **app-image**). Másold az
SD-kártya `/apps/` mappájába `.bin` néven; a launcher listázza és indítja.

> ⚠️ A `firmware.bin`-t másold — NE a `bootloader.bin`/`partitions.bin`-t, és NE
> „merged/full flash" képet.
