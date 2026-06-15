/**
 * @file lv_conf.h
 * LVGL 9.x konfiguráció a Sunton ESP32-2432S022C (CYD 2.2") board-hoz.
 *
 * Csak a board szempontjából lényeges opciókat állítjuk be; minden mást az
 * LVGL belső alapértelmezett értéke (lv_conf_internal.h) tölt ki. A fájlt az
 * esp32-smartdisplay a -D LV_CONF_PATH build flag-en keresztül találja meg.
 */

/* clang-format off */
#ifndef LV_CONF_H
#define LV_CONF_H

/* Megj.: itt szándékosan NEM include-olunk <stdint.h>-t. Ezt a fájlt az LVGL
 * .S (assembly) forrásai is beemelik a lv_conf_internal.h-n keresztül, és a
 * GCC assembler elhasalna a stdint typedef-eken. C típusra nincs is szükség. */

/*====================
   SZÍN BEÁLLÍTÁSOK
 *====================*/

/* ST7789: 16 bites szín, R5G6B5 */
#define LV_COLOR_DEPTH 16

/*=========================
   MEMÓRIA BEÁLLÍTÁSOK
 *=========================*/

/* Az ESP32-WROOM-32-n nincs PSRAM, ezért a belső heap-et használjuk. */
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN

/* LVGL által kezelt heap mérete (byte). 48 KB jó kompromisszum a 022C-n. */
#define LV_MEM_SIZE (48U * 1024U)

/*====================
   HAL / IDŐZÍTÉS
 *====================*/

/* Frissítési és input olvasási periódus (ms). */
#define LV_DEF_REFR_PERIOD 16        /* ~60 FPS célzott frissítés */
#define LV_DPI_DEF 130               /* 2.2" 240x320 ~ 182 DPI, 130 jó alap UI-hoz */

/* A tick-et a main.cpp loop()-ja adja meg lv_tick_inc()-cel. */

/*=================
   OPERÁCIÓS RDSZ
 *=================*/

#define LV_USE_OS LV_OS_NONE

/*========================
   RENDERELÉS / DRAW
 *========================*/

#define LV_DRAW_BUF_STRIDE_ALIGN 1
#define LV_DRAW_BUF_ALIGN 4

/*=======================
   LOG BEÁLLÍTÁSOK
 *=======================*/

#define LV_USE_LOG 1
#if LV_USE_LOG
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
    #define LV_LOG_PRINTF 1
#endif

/*=======================
   HIBAKERESÉS / ASSERT
 *=======================*/

#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0

/*==================
   FONTOK
 *==================*/

#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_24 1

#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*==================
   WIDGET-EK
 *==================*/
/* Az LVGL alapból minden widgetet bekapcsol; itt nem tiltunk le semmit. */

/*==================
   DEMÓK
 *==================*/
/* A beépített demók a saját UI mellett kipróbálhatók (lásd main.cpp). */
#define LV_USE_DEMO_WIDGETS 1
#define LV_USE_DEMO_BENCHMARK 1

#endif /* LV_CONF_H */
