/**
 * @file lv_conf.h
 * LVGL 9.x konfiguráció a grafikus SD-bootloaderhez (ESP32-2432S022C).
 * Megegyezik a fő projekt configjával, de a beépített demók kikapcsolva, hogy a
 * launcher kicsi maradjon (a factory partícióba kell férnie).
 */

/* clang-format off */
#ifndef LV_CONF_H
#define LV_CONF_H

/* Megj.: szándékosan NINCS <stdint.h> include — az LVGL .S forrásai is beemelik
 * ezt a fájlt, és a GCC assembler elhasalna a stdint typedef-eken. */

/* Szín: ST7789, 16 bit (R5G6B5) */
#define LV_COLOR_DEPTH 16

/* Memória: nincs PSRAM, belső heap */
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN
#define LV_MEM_SIZE (48U * 1024U)

/* HAL / időzítés (a tick-et a loop() adja lv_tick_inc-cel) */
#define LV_DEF_REFR_PERIOD 16
#define LV_DPI_DEF 130

#define LV_USE_OS LV_OS_NONE

#define LV_DRAW_BUF_STRIDE_ALIGN 1
#define LV_DRAW_BUF_ALIGN 4

/* Log */
#define LV_USE_LOG 1
#if LV_USE_LOG
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
    #define LV_LOG_PRINTF 1
#endif

/* Assert */
#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0

/* Fontok */
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/* Demók nem kellenek a launcherhez */
#define LV_USE_DEMO_WIDGETS 0
#define LV_USE_DEMO_BENCHMARK 0

#endif /* LV_CONF_H */
