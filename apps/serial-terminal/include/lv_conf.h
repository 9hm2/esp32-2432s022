/**
 * @file lv_conf.h
 * LVGL 9.x konfiguráció a serial-terminal apphoz (ESP32-2432S022C).
 * Mint a sablon, plusz egy monospace (UNSCII) font a terminál-megjelenítéshez.
 */

/* clang-format off */
#ifndef LV_CONF_H
#define LV_CONF_H

/* Megj.: szándékosan NINCS <stdint.h> include (lásd a többi projekt magyarázatát). */

#define LV_COLOR_DEPTH 16

#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN
#define LV_MEM_SIZE (48U * 1024U)

#define LV_DEF_REFR_PERIOD 16
#define LV_DPI_DEF 130

#define LV_USE_OS LV_OS_NONE

#define LV_DRAW_BUF_STRIDE_ALIGN 1
#define LV_DRAW_BUF_ALIGN 4

#define LV_USE_LOG 1
#if LV_USE_LOG
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
    #define LV_LOG_PRINTF 1
#endif

#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0

/* Fontok: arányos a UI-hoz + monospace a terminálhoz */
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_UNSCII_8 1            /* monospace 8x8 - terminál look */
#define LV_FONT_DEFAULT &lv_font_montserrat_14

#define LV_USE_DEMO_WIDGETS 0
#define LV_USE_DEMO_BENCHMARK 0

#endif /* LV_CONF_H */
