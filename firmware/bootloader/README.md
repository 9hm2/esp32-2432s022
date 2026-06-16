# Grafikus SD-bootloader (launcher) — firmware

Az ESP32-2432S022C-re készülő, érintőképernyős **app-indító**. SD-kártyán lévő
`.bin` firmware-fájlokat listáz és futtat (OTA flash-and-boot).

A teljes tervet lásd: [`../../docs/BOOTLOADER_PLAN.md`](../../docs/BOOTLOADER_PLAN.md).

## Állapot: M2 ✅

| Mérföldkő | Tartalom | Állapot |
|---|---|---|
| **M1** | partíciótábla + SD mount + `.bin` listázás soros porton | ✅ kész |
| **M2** | LVGL lista GUI (érintéssel) | ✅ kész (fordul) |
| M3 | OTA flash-and-boot (PoC) | hátravan |
| M4 | teljes folyamat GUI-val | hátravan |
| M5 | vissza a menübe (rollback + kooperatív) | hátravan |
| M6 | csiszolás | hátravan |

## Mit csinál most (M2)

Bootkor a launcher (factory partíció):
1. soros portra kiírja a partíció-elrendezést (futó / ota_0),
2. inicializálja a kijelzőt + touch-ot + LVGL-t (smartdisplay),
3. csatolja az SD-kártyát (SPI: CS=5, SCK=18, MISO=19, MOSI=23),
4. **érintőképernyős listában** mutatja az `/apps/*.bin` fájlokat (név + méret),
5. koppintásra visszajelez (a tényleges flashelés az M3).

Hibák (nincs SD / nincs `.bin`) modális üzenetben jelennek meg.

## SD-kártya elrendezés

- Formátum: **FAT32**.
- Az app-fájlokat tedd a kártyára: `/apps/valami.bin` (vagy a gyökérbe).
- A `.bin` az **app-image** legyen (a normál build `0x10000`-re kerülő `.bin`-je),
  **nem** a „merged/full flash" kép. Részletek a tervben.

## Build & feltöltés

```bash
cd firmware/bootloader
pio run                 # fordítás
pio run -t upload       # feltöltés a panelra
pio device monitor      # 115200 baud, a listát itt látod
```

A board-definíció a repó gyökerében lévő `boards/` mappából jön
(`boards_dir = ../../boards`), a partíciótábla a `partitions.csv`.

## Partíciótábla (4 MB)

| Név | Típus | Offset | Méret | Szerep |
|---|---|---|---|---|
| nvs | data/nvs | 0x9000 | 20 KB | beállítások |
| otadata | data/ota | 0xe000 | 8 KB | OTA boot-állapot |
| phy_init | data/phy | 0x10000 | 4 KB | RF kalibráció |
| **factory** | app | 0x20000 | 1.5 MB | **launcher** |
| **ota_0** | app | 0x1a0000 | ~2.3 MB | **futtatott app** |
