/*******************************************************************************
 * Size: 8 px
 * Bpp: 1
 * Opts: --font /usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf --size 8 --bpp 1 --range 0x20-0x7E,0xA0-0xFF,0x2500-0x259F --format lvgl --no-compress -o src/term_font.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef TERM_FONT
#define TERM_FONT 1
#endif

#if TERM_FONT

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xf2,

    /* U+0022 "\"" */
    0xf0,

    /* U+0023 "#" */
    0x32, 0xbe, 0xaf, 0x32, 0x80,

    /* U+0024 "$" */
    0x4e, 0xcc, 0x75, 0xe4, 0x40,

    /* U+0025 "%" */
    0xe5, 0x3c, 0xf2, 0x9c,

    /* U+0026 "&" */
    0x62, 0x11, 0x4a, 0xc9, 0xa0,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0x6a, 0xa9,

    /* U+0029 ")" */
    0xa5, 0x5a,

    /* U+002A "*" */
    0x23, 0x9c, 0x40,

    /* U+002B "+" */
    0x27, 0xc8, 0x40,

    /* U+002C "," */
    0xc0,

    /* U+002D "-" */
    0xc0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x11, 0x22, 0x44, 0x48,

    /* U+0030 "0" */
    0x69, 0x9b, 0x99, 0x60,

    /* U+0031 "1" */
    0xc9, 0x24, 0xb8,

    /* U+0032 "2" */
    0x61, 0x12, 0x24, 0xf0,

    /* U+0033 "3" */
    0xe1, 0x16, 0x11, 0xe0,

    /* U+0034 "4" */
    0x11, 0x94, 0xa9, 0x7c, 0x40,

    /* U+0035 "5" */
    0xd3, 0x12, 0x70,

    /* U+0036 "6" */
    0x68, 0xe9, 0x99, 0x60,

    /* U+0037 "7" */
    0xf1, 0x22, 0x24, 0x40,

    /* U+0038 "8" */
    0x69, 0x96, 0x99, 0x60,

    /* U+0039 "9" */
    0x69, 0x99, 0x71, 0x60,

    /* U+003A ":" */
    0x90,

    /* U+003B ";" */
    0x8c,

    /* U+003C "<" */
    0x16, 0x87,

    /* U+003D "=" */
    0xf0, 0xf0,

    /* U+003E ">" */
    0x86, 0x1e,

    /* U+003F "?" */
    0xe4, 0xa4, 0x10,

    /* U+0040 "@" */
    0x72, 0x6f, 0x9c, 0xdd, 0xe,

    /* U+0041 "A" */
    0x23, 0x14, 0xa7, 0x4a, 0x20,

    /* U+0042 "B" */
    0xe9, 0x9e, 0x99, 0xe0,

    /* U+0043 "C" */
    0x7c, 0x88, 0x8c, 0x70,

    /* U+0044 "D" */
    0xe9, 0x99, 0x99, 0xe0,

    /* U+0045 "E" */
    0xf8, 0x8f, 0x88, 0xf0,

    /* U+0046 "F" */
    0xf8, 0x8e, 0x88, 0x80,

    /* U+0047 "G" */
    0x78, 0x8b, 0x99, 0x70,

    /* U+0048 "H" */
    0x99, 0x9f, 0x99, 0x90,

    /* U+0049 "I" */
    0xe9, 0x24, 0xb8,

    /* U+004A "J" */
    0x71, 0x11, 0x19, 0x70,

    /* U+004B "K" */
    0x9a, 0xcc, 0xa9, 0x90,

    /* U+004C "L" */
    0x88, 0x88, 0x88, 0xf0,

    /* U+004D "M" */
    0x9f, 0xfd, 0x99, 0x90,

    /* U+004E "N" */
    0x9d, 0xdd, 0xbb, 0x90,

    /* U+004F "O" */
    0x69, 0x99, 0x99, 0x60,

    /* U+0050 "P" */
    0xe9, 0x99, 0xe8, 0x80,

    /* U+0051 "Q" */
    0x69, 0x99, 0x99, 0x61,

    /* U+0052 "R" */
    0xe4, 0xa5, 0xc9, 0x4a, 0x40,

    /* U+0053 "S" */
    0x78, 0x86, 0x11, 0xe0,

    /* U+0054 "T" */
    0xf9, 0x8, 0x42, 0x10, 0x80,

    /* U+0055 "U" */
    0x99, 0x99, 0x99, 0x60,

    /* U+0056 "V" */
    0x94, 0x94, 0xa5, 0x10, 0x80,

    /* U+0057 "W" */
    0x8c, 0x6b, 0xef, 0x29, 0x40,

    /* U+0058 "X" */
    0x92, 0x98, 0x46, 0x2a, 0x20,

    /* U+0059 "Y" */
    0x8a, 0x94, 0x42, 0x10, 0x80,

    /* U+005A "Z" */
    0xf1, 0x24, 0x48, 0xf0,

    /* U+005B "[" */
    0xea, 0xaa, 0xc0,

    /* U+005C "\\" */
    0x84, 0x42, 0x22, 0x11,

    /* U+005D "]" */
    0xd5, 0x55, 0xc0,

    /* U+005E "^" */
    0x23, 0x24,

    /* U+005F "_" */
    0xf8,

    /* U+0060 "`" */
    0x90,

    /* U+0061 "a" */
    0xe1, 0xf9, 0xf0,

    /* U+0062 "b" */
    0x88, 0xe9, 0x99, 0xe0,

    /* U+0063 "c" */
    0x78, 0x88, 0x70,

    /* U+0064 "d" */
    0x11, 0x79, 0x99, 0x70,

    /* U+0065 "e" */
    0x69, 0xf8, 0x70,

    /* U+0066 "f" */
    0x74, 0xf4, 0x44, 0x40,

    /* U+0067 "g" */
    0x79, 0x99, 0x71, 0x60,

    /* U+0068 "h" */
    0x93, 0xdb, 0x68,

    /* U+0069 "i" */
    0x43, 0x24, 0xb8,

    /* U+006A "j" */
    0x21, 0x92, 0x49, 0xe0,

    /* U+006B "k" */
    0x88, 0xac, 0xca, 0x90,

    /* U+006C "l" */
    0xc4, 0x44, 0x44, 0x70,

    /* U+006D "m" */
    0xfd, 0x6b, 0x5a, 0x80,

    /* U+006E "n" */
    0xf6, 0xda,

    /* U+006F "o" */
    0x69, 0x99, 0x60,

    /* U+0070 "p" */
    0xe9, 0x99, 0xe8, 0x80,

    /* U+0071 "q" */
    0x79, 0x99, 0x71, 0x10,

    /* U+0072 "r" */
    0xf2, 0x48,

    /* U+0073 "s" */
    0xf1, 0x1e,

    /* U+0074 "t" */
    0x44, 0xf4, 0x44, 0x70,

    /* U+0075 "u" */
    0xb6, 0xde,

    /* U+0076 "v" */
    0x95, 0x56, 0x20,

    /* U+0077 "w" */
    0x8d, 0x6d, 0xa5, 0x0,

    /* U+0078 "x" */
    0x53, 0x8, 0xa9, 0x0,

    /* U+0079 "y" */
    0x92, 0x94, 0xc2, 0x11, 0x0,

    /* U+007A "z" */
    0xf2, 0x48, 0xf0,

    /* U+007B "{" */
    0x69, 0x28, 0x92, 0x60,

    /* U+007C "|" */
    0xff, 0x80,

    /* U+007D "}" */
    0xc9, 0x22, 0x92, 0xc0,

    /* U+007E "~" */
    0xf0,

    /* U+00A0 " " */
    0x0,

    /* U+00A1 "¡" */
    0xbc,

    /* U+00A2 "¢" */
    0x22, 0x7a, 0xaa, 0x72,

    /* U+00A3 "£" */
    0x74, 0x4e, 0x44, 0xf0,

    /* U+00A4 "¤" */
    0xf7, 0x80,

    /* U+00A5 "¥" */
    0x8a, 0x95, 0xbf, 0x90, 0x80,

    /* U+00A6 "¦" */
    0xe7,

    /* U+00A7 "§" */
    0xf1, 0x5a, 0x8f,

    /* U+00A8 "¨" */
    0xa0,

    /* U+00A9 "©" */
    0x77, 0xf3, 0x9f, 0xb8,

    /* U+00AA "ª" */
    0xdf, 0xa0,

    /* U+00AB "«" */
    0x16, 0xa6,

    /* U+00AC "¬" */
    0xf1, 0x10,

    /* U+00AD "­" */
    0xc0,

    /* U+00AE "®" */
    0x77, 0xfb, 0xdd, 0xb8,

    /* U+00AF "¯" */
    0xe0,

    /* U+00B0 "°" */
    0xf7, 0x80,

    /* U+00B1 "±" */
    0x27, 0xc8, 0xf, 0x80,

    /* U+00B2 "²" */
    0x58,

    /* U+00B3 "³" */
    0x74,

    /* U+00B4 "´" */
    0x60,

    /* U+00B5 "µ" */
    0xaa, 0xaa, 0xf8, 0x80,

    /* U+00B6 "¶" */
    0x77, 0x77, 0x33, 0x33,

    /* U+00B7 "·" */
    0x80,

    /* U+00B8 "¸" */
    0x70,

    /* U+00B9 "¹" */
    0xc9, 0x0,

    /* U+00BA "º" */
    0xd6, 0xa0,

    /* U+00BB "»" */
    0xa, 0x5a,

    /* U+00BC "¼" */
    0x42, 0x11, 0xc7, 0x48, 0xc6, 0x10,

    /* U+00BD "½" */
    0x44, 0x4e, 0x7b, 0x12, 0x30,

    /* U+00BE "¾" */
    0xc2, 0x11, 0x87, 0x49, 0xce, 0x10,

    /* U+00BF "¿" */
    0x41, 0x29, 0x38,

    /* U+00C0 "À" */
    0x41, 0x8, 0xc5, 0x29, 0xd2, 0x88,

    /* U+00C1 "Á" */
    0x1, 0x8, 0xc5, 0x29, 0xd2, 0x88,

    /* U+00C2 "Â" */
    0x23, 0x8, 0xc5, 0x29, 0xd2, 0x88,

    /* U+00C3 "Ã" */
    0x71, 0x18, 0xa5, 0x3a, 0x51,

    /* U+00C4 "Ä" */
    0x51, 0x18, 0xa5, 0x3a, 0x51,

    /* U+00C5 "Å" */
    0x66, 0x66, 0x6a, 0xf9, 0x90,

    /* U+00C6 "Æ" */
    0x7a, 0xab, 0xea, 0xb0,

    /* U+00C7 "Ç" */
    0x78, 0x88, 0x88, 0x70, 0x20,

    /* U+00C8 "È" */
    0x44, 0xf8, 0x8f, 0x88, 0xf0,

    /* U+00C9 "É" */
    0x24, 0xf8, 0x8f, 0x88, 0xf0,

    /* U+00CA "Ê" */
    0x6, 0xf8, 0x8f, 0x88, 0xf0,

    /* U+00CB "Ë" */
    0x6f, 0x88, 0xf8, 0x8f,

    /* U+00CC "Ì" */
    0x8b, 0xa4, 0x92, 0xe0,

    /* U+00CD "Í" */
    0x2b, 0xa4, 0x92, 0xe0,

    /* U+00CE "Î" */
    0x57, 0xa4, 0x92, 0xe0,

    /* U+00CF "Ï" */
    0xbd, 0x24, 0x97,

    /* U+00D0 "Ð" */
    0xe9, 0x9d, 0x99, 0xe0,

    /* U+00D1 "Ñ" */
    0x69, 0xdd, 0xdb, 0xb9,

    /* U+00D2 "Ò" */
    0x40, 0x69, 0x99, 0x99, 0x60,

    /* U+00D3 "Ó" */
    0x20, 0x69, 0x99, 0x99, 0x60,

    /* U+00D4 "Ô" */
    0x6, 0x69, 0x99, 0x99, 0x60,

    /* U+00D5 "Õ" */
    0x6a, 0x69, 0x99, 0x99, 0x60,

    /* U+00D6 "Ö" */
    0x66, 0x99, 0x99, 0x96,

    /* U+00D7 "×" */
    0x35, 0x50,

    /* U+00D8 "Ø" */
    0x79, 0xbd, 0xd9, 0xe0,

    /* U+00D9 "Ù" */
    0x42, 0x99, 0x99, 0x99, 0x60,

    /* U+00DA "Ú" */
    0x24, 0x99, 0x99, 0x99, 0x60,

    /* U+00DB "Û" */
    0x6, 0x99, 0x99, 0x99, 0x60,

    /* U+00DC "Ü" */
    0x69, 0x99, 0x99, 0x96,

    /* U+00DD "Ý" */
    0x11, 0x22, 0xa5, 0x10, 0x84, 0x20,

    /* U+00DE "Þ" */
    0x88, 0xf9, 0x9f, 0x80,

    /* U+00DF "ß" */
    0xea, 0xcc, 0xb9, 0xf0,

    /* U+00E0 "à" */
    0x42, 0xe1, 0xf9, 0xf0,

    /* U+00E1 "á" */
    0x20, 0xe1, 0xf9, 0xf0,

    /* U+00E2 "â" */
    0x60, 0xe1, 0xf9, 0xf0,

    /* U+00E3 "ã" */
    0x6a, 0xe1, 0xf9, 0xf0,

    /* U+00E4 "ä" */
    0x60, 0xe1, 0xf9, 0xf0,

    /* U+00E5 "å" */
    0x66, 0x6e, 0x1f, 0x9f,

    /* U+00E6 "æ" */
    0xd9, 0x7f, 0x4d, 0x80,

    /* U+00E7 "ç" */
    0x78, 0x88, 0x62, 0x60,

    /* U+00E8 "è" */
    0x44, 0x26, 0x9f, 0x87,

    /* U+00E9 "é" */
    0x22, 0x6, 0x9f, 0x87,

    /* U+00EA "ê" */
    0x6, 0x6, 0x9f, 0x87,

    /* U+00EB "ë" */
    0x60, 0x69, 0xf8, 0x70,

    /* U+00EC "ì" */
    0x8b, 0x24, 0xb8,

    /* U+00ED "í" */
    0x43, 0x24, 0xb8,

    /* U+00EE "î" */
    0x57, 0x24, 0xb8,

    /* U+00EF "ï" */
    0xa3, 0x24, 0xb8,

    /* U+00F0 "ð" */
    0x62, 0x69, 0x99, 0x60,

    /* U+00F1 "ñ" */
    0xdb, 0xdb, 0x68,

    /* U+00F2 "ò" */
    0x44, 0x26, 0x99, 0x96,

    /* U+00F3 "ó" */
    0x22, 0x46, 0x99, 0x96,

    /* U+00F4 "ô" */
    0x6, 0x6, 0x99, 0x96,

    /* U+00F5 "õ" */
    0x60, 0x69, 0x99, 0x60,

    /* U+00F6 "ö" */
    0x60, 0x69, 0x99, 0x60,

    /* U+00F7 "÷" */
    0x20, 0x3e, 0x40,

    /* U+00F8 "ø" */
    0x79, 0xf9, 0xe0,

    /* U+00F9 "ù" */
    0x8a, 0xdb, 0x78,

    /* U+00FA "ú" */
    0x4a, 0xdb, 0x78,

    /* U+00FB "û" */
    0x42, 0xdb, 0x78,

    /* U+00FC "ü" */
    0xc2, 0xdb, 0x78,

    /* U+00FD "ý" */
    0x20, 0x24, 0xa5, 0x30, 0x84, 0x40,

    /* U+00FE "þ" */
    0x88, 0xe9, 0x99, 0xe8, 0x80,

    /* U+00FF "ÿ" */
    0x50, 0x24, 0xa5, 0x30, 0x84, 0x40,

    /* U+2500 "─" */
    0xf8,

    /* U+2501 "━" */
    0xf8,

    /* U+2502 "│" */
    0xff, 0x80,

    /* U+2503 "┃" */
    0xff, 0x80,

    /* U+2504 "┄" */
    0xa8,

    /* U+2505 "┅" */
    0xa8,

    /* U+2506 "┆" */
    0xdb,

    /* U+2507 "┇" */
    0xdb,

    /* U+2508 "┈" */
    0xf0,

    /* U+2509 "┉" */
    0xf0,

    /* U+250A "┊" */
    0x95,

    /* U+250B "┋" */
    0x95,

    /* U+250C "┌" */
    0xf2, 0x48,

    /* U+250D "┍" */
    0xf2, 0x48,

    /* U+250E "┎" */
    0xf2, 0x48,

    /* U+250F "┏" */
    0xf2, 0x48,

    /* U+2510 "┐" */
    0xe4, 0x92,

    /* U+2511 "┑" */
    0xe4, 0x92,

    /* U+2512 "┒" */
    0xe4, 0x92,

    /* U+2513 "┓" */
    0xe4, 0x92,

    /* U+2514 "└" */
    0x92, 0x4e,

    /* U+2515 "┕" */
    0x92, 0x4e,

    /* U+2516 "┖" */
    0x92, 0x4e,

    /* U+2517 "┗" */
    0x92, 0x4e,

    /* U+2518 "┘" */
    0x24, 0x9e,

    /* U+2519 "┙" */
    0x24, 0x9e,

    /* U+251A "┚" */
    0x24, 0x9e,

    /* U+251B "┛" */
    0x24, 0x9e,

    /* U+251C "├" */
    0x92, 0x4f, 0x24, 0x80,

    /* U+251D "┝" */
    0x92, 0x4f, 0x24, 0x80,

    /* U+251E "┞" */
    0x92, 0x4f, 0x24, 0x80,

    /* U+251F "┟" */
    0x92, 0x4f, 0x24, 0x80,

    /* U+2520 "┠" */
    0x92, 0x4f, 0x24, 0x80,

    /* U+2521 "┡" */
    0x92, 0x4f, 0x24, 0x80,

    /* U+2522 "┢" */
    0x92, 0x4f, 0x24, 0x80,

    /* U+2523 "┣" */
    0x92, 0x4f, 0x24, 0x80,

    /* U+2524 "┤" */
    0x24, 0x9e, 0x49, 0x20,

    /* U+2525 "┥" */
    0x24, 0x9e, 0x49, 0x20,

    /* U+2526 "┦" */
    0x24, 0x9e, 0x49, 0x20,

    /* U+2527 "┧" */
    0x24, 0x9e, 0x49, 0x20,

    /* U+2528 "┨" */
    0x24, 0x9e, 0x49, 0x20,

    /* U+2529 "┩" */
    0x24, 0x9e, 0x49, 0x20,

    /* U+252A "┪" */
    0x24, 0x9e, 0x49, 0x20,

    /* U+252B "┫" */
    0x24, 0x9e, 0x49, 0x20,

    /* U+252C "┬" */
    0xf9, 0x8, 0x42, 0x0,

    /* U+252D "┭" */
    0xf9, 0x8, 0x42, 0x0,

    /* U+252E "┮" */
    0xf9, 0x8, 0x42, 0x0,

    /* U+252F "┯" */
    0xf9, 0x8, 0x42, 0x0,

    /* U+2530 "┰" */
    0xf9, 0x8, 0x42, 0x0,

    /* U+2531 "┱" */
    0xf9, 0x8, 0x42, 0x0,

    /* U+2532 "┲" */
    0xf9, 0x8, 0x42, 0x0,

    /* U+2533 "┳" */
    0xf9, 0x8, 0x42, 0x0,

    /* U+2534 "┴" */
    0x21, 0x8, 0x4f, 0x80,

    /* U+2535 "┵" */
    0x21, 0x8, 0x4f, 0x80,

    /* U+2536 "┶" */
    0x21, 0x8, 0x4f, 0x80,

    /* U+2537 "┷" */
    0x21, 0x8, 0x4f, 0x80,

    /* U+2538 "┸" */
    0x21, 0x8, 0x4f, 0x80,

    /* U+2539 "┹" */
    0x21, 0x8, 0x4f, 0x80,

    /* U+253A "┺" */
    0x21, 0x8, 0x4f, 0x80,

    /* U+253B "┻" */
    0x21, 0x8, 0x4f, 0x80,

    /* U+253C "┼" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+253D "┽" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+253E "┾" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+253F "┿" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+2540 "╀" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+2541 "╁" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+2542 "╂" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+2543 "╃" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+2544 "╄" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+2545 "╅" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+2546 "╆" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+2547 "╇" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+2548 "╈" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+2549 "╉" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+254A "╊" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+254B "╋" */
    0x21, 0x8, 0x4f, 0x90, 0x84, 0x20,

    /* U+254C "╌" */
    0xf0,

    /* U+254D "╍" */
    0xf0,

    /* U+254E "╎" */
    0xe7,

    /* U+254F "╏" */
    0xe7,

    /* U+2550 "═" */
    0xff, 0xc0,

    /* U+2551 "║" */
    0xff, 0xff, 0xc0,

    /* U+2552 "╒" */
    0xfe, 0x49, 0x0,

    /* U+2553 "╓" */
    0xfc, 0xcc, 0xc0,

    /* U+2554 "╔" */
    0xeb, 0xcc, 0xc8,

    /* U+2555 "╕" */
    0xfc, 0x92, 0x40,

    /* U+2556 "╖" */
    0xed, 0xb6,

    /* U+2557 "╗" */
    0x7f, 0x33, 0x32,

    /* U+2558 "╘" */
    0x92, 0x4f, 0xc0,

    /* U+2559 "╙" */
    0xcc, 0xcc, 0xf0,

    /* U+255A "╚" */
    0x8c, 0xcc, 0xaf,

    /* U+255B "╛" */
    0x24, 0x9f, 0xc0,

    /* U+255C "╜" */
    0x6d, 0xbe,

    /* U+255D "╝" */
    0x23, 0x33, 0x5f,

    /* U+255E "╞" */
    0x92, 0x4f, 0xe4, 0x80,

    /* U+255F "╟" */
    0xcc, 0xcc, 0xbc, 0xcc, 0xc0,

    /* U+2560 "╠" */
    0xcc, 0xcc, 0xae, 0xcc, 0xc0,

    /* U+2561 "╡" */
    0x24, 0x9f, 0xc9, 0x20,

    /* U+2562 "╢" */
    0x33, 0x33, 0xf3, 0x33, 0x30,

    /* U+2563 "╣" */
    0x33, 0x33, 0x57, 0x33, 0x30,

    /* U+2564 "╤" */
    0xff, 0xc8, 0x42, 0x10,

    /* U+2565 "╥" */
    0xfb, 0x18, 0xc6, 0x0,

    /* U+2566 "╦" */
    0xfe, 0xd8, 0xc6, 0x0,

    /* U+2567 "╧" */
    0x21, 0x8, 0x4d, 0xfc,

    /* U+2568 "╨" */
    0x63, 0x18, 0xcf, 0x80,

    /* U+2569 "╩" */
    0x3, 0x18, 0xc9, 0xfc,

    /* U+256A "╪" */
    0x21, 0x8, 0x4f, 0xfc, 0x84, 0x20,

    /* U+256B "╫" */
    0x63, 0x18, 0xcf, 0xb1, 0x8c, 0x60,

    /* U+256C "╬" */
    0x20, 0xc3, 0xc, 0x4b, 0xb3, 0xc, 0x30, 0x80,

    /* U+256D "╭" */
    0xf2, 0x48,

    /* U+256E "╮" */
    0xe4, 0x92,

    /* U+256F "╯" */
    0x24, 0x9e,

    /* U+2570 "╰" */
    0x92, 0x4e,

    /* U+2571 "╱" */
    0x8, 0x84, 0x42, 0x21, 0x10, 0x80,

    /* U+2572 "╲" */
    0x84, 0x10, 0x82, 0x10, 0x42, 0x8,

    /* U+2573 "╳" */
    0x8c, 0x94, 0xc2, 0x31, 0x52, 0x88,

    /* U+2574 "╴" */
    0xc0,

    /* U+2575 "╵" */
    0xf8,

    /* U+2576 "╶" */
    0xc0,

    /* U+2577 "╷" */
    0xf8,

    /* U+2578 "╸" */
    0xc0,

    /* U+2579 "╹" */
    0xf8,

    /* U+257A "╺" */
    0xc0,

    /* U+257B "╻" */
    0xf8,

    /* U+257C "╼" */
    0xf8,

    /* U+257D "╽" */
    0xff, 0xc0,

    /* U+257E "╾" */
    0xf8,

    /* U+257F "╿" */
    0xff, 0xc0,

    /* U+2580 "▀" */
    0xff, 0xff, 0xff, 0x80,

    /* U+2581 "▁" */
    0xf8,

    /* U+2582 "▂" */
    0xff, 0xc0,

    /* U+2583 "▃" */
    0xff, 0xfe,

    /* U+2584 "▄" */
    0xff, 0xff, 0xff, 0x80,

    /* U+2585 "▅" */
    0xff, 0xff, 0xff, 0xfc,

    /* U+2586 "▆" */
    0xff, 0xff, 0xff, 0xff, 0xe0,

    /* U+2587 "▇" */
    0xff, 0xff, 0xff, 0xff, 0xff,

    /* U+2588 "█" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,

    /* U+2589 "▉" */
    0xff, 0xff, 0xff, 0xff, 0xf0,

    /* U+258A "▊" */
    0xff, 0xff, 0xff, 0xff, 0xf0,

    /* U+258B "▋" */
    0xff, 0xff, 0xff, 0xe0,

    /* U+258C "▌" */
    0xff, 0xff, 0xc0,

    /* U+258D "▍" */
    0xff, 0xff, 0xc0,

    /* U+258E "▎" */
    0xff, 0x80,

    /* U+258F "▏" */
    0xff, 0x80,

    /* U+2590 "▐" */
    0xff, 0xff, 0xc0,

    /* U+2591 "░" */
    0xa5, 0xa5, 0xaa, 0x5a, 0x50,

    /* U+2592 "▒" */
    0xd7, 0xdb, 0xa6, 0xe9, 0xba, 0x68,

    /* U+2593 "▓" */
    0xdd, 0x77, 0x5d, 0xee, 0xbb, 0xa8,

    /* U+2594 "▔" */
    0xf8,

    /* U+2595 "▕" */
    0xff, 0x80,

    /* U+2596 "▖" */
    0xff, 0xc0,

    /* U+2597 "▗" */
    0xff, 0xfe,

    /* U+2598 "▘" */
    0xff, 0xc0,

    /* U+2599 "▙" */
    0xc6, 0x31, 0x8c, 0x7f, 0xff, 0xff, 0xc0,

    /* U+259A "▚" */
    0xc6, 0x31, 0x8c, 0x1c, 0xe7, 0x39, 0xc0,

    /* U+259B "▛" */
    0xff, 0xff, 0xff, 0xe3, 0x18, 0xc6, 0x0,

    /* U+259C "▜" */
    0xff, 0xff, 0xff, 0x9c, 0xe7, 0x39, 0xc0,

    /* U+259D "▝" */
    0xff, 0xfe,

    /* U+259E "▞" */
    0x39, 0xce, 0x73, 0xe3, 0x18, 0xc6, 0x0,

    /* U+259F "▟" */
    0x39, 0xce, 0x73, 0xff, 0xff, 0xff, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 80, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 80, .box_w = 1, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 80, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 3, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 8, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 13, .adv_w = 80, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 17, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 80, .box_w = 1, .box_h = 2, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 23, .adv_w = 80, .box_w = 2, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 25, .adv_w = 80, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 27, .adv_w = 80, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 30, .adv_w = 80, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 33, .adv_w = 80, .box_w = 1, .box_h = 2, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 34, .adv_w = 80, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 35, .adv_w = 80, .box_w = 1, .box_h = 1, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 36, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 40, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 47, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 51, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 55, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 63, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 67, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 71, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 75, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 79, .adv_w = 80, .box_w = 1, .box_h = 4, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 80, .box_w = 1, .box_h = 6, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 81, .adv_w = 80, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 83, .adv_w = 80, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 85, .adv_w = 80, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 87, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 90, .adv_w = 80, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 95, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 108, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 112, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 116, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 124, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 143, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 147, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 155, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 159, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 163, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 168, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 181, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 196, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 205, .adv_w = 80, .box_w = 2, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 208, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 212, .adv_w = 80, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 215, .adv_w = 80, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 217, .adv_w = 80, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 218, .adv_w = 80, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 219, .adv_w = 80, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 80, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 80, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 236, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 244, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 250, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 254, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 258, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 262, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 266, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 268, .adv_w = 80, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 271, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 275, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 279, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 283, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 287, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 80, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 292, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 300, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 305, .adv_w = 80, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 312, .adv_w = 80, .box_w = 1, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 314, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 318, .adv_w = 80, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 319, .adv_w = 80, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 320, .adv_w = 80, .box_w = 1, .box_h = 6, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 321, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 325, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 329, .adv_w = 80, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 331, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 336, .adv_w = 80, .box_w = 1, .box_h = 8, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 337, .adv_w = 80, .box_w = 3, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 340, .adv_w = 80, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 341, .adv_w = 80, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 345, .adv_w = 80, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 347, .adv_w = 80, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 349, .adv_w = 80, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 351, .adv_w = 80, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 352, .adv_w = 80, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 356, .adv_w = 80, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 357, .adv_w = 80, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 359, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 363, .adv_w = 80, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 364, .adv_w = 80, .box_w = 2, .box_h = 3, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 365, .adv_w = 80, .box_w = 2, .box_h = 2, .ofs_x = 2, .ofs_y = 6},
    {.bitmap_index = 366, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 370, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 374, .adv_w = 80, .box_w = 1, .box_h = 1, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 375, .adv_w = 80, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 376, .adv_w = 80, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 378, .adv_w = 80, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 380, .adv_w = 80, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 382, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 388, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 393, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 399, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 402, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 408, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 414, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 420, .adv_w = 80, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 425, .adv_w = 80, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 430, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 435, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 439, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 444, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 449, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 454, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 459, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 463, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 467, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 471, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 475, .adv_w = 80, .box_w = 3, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 478, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 482, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 486, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 491, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 496, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 501, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 506, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 510, .adv_w = 80, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 512, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 516, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 521, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 526, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 531, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 535, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 541, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 545, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 549, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 553, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 557, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 561, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 565, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 569, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 573, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 577, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 581, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 585, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 589, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 593, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 597, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 600, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 603, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 606, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 609, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 613, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 616, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 620, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 624, .adv_w = 80, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 628, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 632, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 636, .adv_w = 80, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 639, .adv_w = 80, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 642, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 645, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 648, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 651, .adv_w = 80, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 654, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 660, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 665, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 671, .adv_w = 80, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 672, .adv_w = 80, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 673, .adv_w = 80, .box_w = 1, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 675, .adv_w = 80, .box_w = 1, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 677, .adv_w = 80, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 678, .adv_w = 80, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 679, .adv_w = 80, .box_w = 1, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 680, .adv_w = 80, .box_w = 1, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 681, .adv_w = 80, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 682, .adv_w = 80, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 683, .adv_w = 80, .box_w = 1, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 684, .adv_w = 80, .box_w = 1, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 685, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 687, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 689, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 691, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 693, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 695, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 697, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 699, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 701, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 703, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 705, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 707, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 709, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 711, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 713, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 715, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 717, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 721, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 725, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 729, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 733, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 737, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 741, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 745, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 749, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 753, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 757, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 761, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 765, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 769, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 773, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 777, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 781, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 785, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 789, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 793, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 797, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 801, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 805, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 809, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 813, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 817, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 821, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 825, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 829, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 833, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 837, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 841, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 845, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 851, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 857, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 863, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 869, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 875, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 881, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 887, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 893, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 899, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 905, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 911, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 917, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 923, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 929, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 935, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 941, .adv_w = 80, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 942, .adv_w = 80, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 943, .adv_w = 80, .box_w = 1, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 944, .adv_w = 80, .box_w = 1, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 945, .adv_w = 80, .box_w = 5, .box_h = 2, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 947, .adv_w = 80, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 950, .adv_w = 80, .box_w = 3, .box_h = 6, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 953, .adv_w = 80, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 956, .adv_w = 80, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 959, .adv_w = 80, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 962, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 964, .adv_w = 80, .box_w = 4, .box_h = 6, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 967, .adv_w = 80, .box_w = 3, .box_h = 6, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 970, .adv_w = 80, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 973, .adv_w = 80, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 976, .adv_w = 80, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 979, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 981, .adv_w = 80, .box_w = 4, .box_h = 6, .ofs_x = -1, .ofs_y = 2},
    {.bitmap_index = 984, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 988, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 993, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 998, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1002, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1007, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1012, .adv_w = 80, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1016, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1020, .adv_w = 80, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1024, .adv_w = 80, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1028, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1032, .adv_w = 80, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1036, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1042, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1048, .adv_w = 80, .box_w = 6, .box_h = 10, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 1056, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1058, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1060, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1062, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 1064, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1070, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1076, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1082, .adv_w = 80, .box_w = 2, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1083, .adv_w = 80, .box_w = 1, .box_h = 5, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 1084, .adv_w = 80, .box_w = 2, .box_h = 1, .ofs_x = 3, .ofs_y = 2},
    {.bitmap_index = 1085, .adv_w = 80, .box_w = 1, .box_h = 5, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1086, .adv_w = 80, .box_w = 2, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1087, .adv_w = 80, .box_w = 1, .box_h = 5, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 1088, .adv_w = 80, .box_w = 2, .box_h = 1, .ofs_x = 3, .ofs_y = 2},
    {.bitmap_index = 1089, .adv_w = 80, .box_w = 1, .box_h = 5, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1090, .adv_w = 80, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1091, .adv_w = 80, .box_w = 1, .box_h = 10, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1093, .adv_w = 80, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1094, .adv_w = 80, .box_w = 1, .box_h = 10, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1096, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 1100, .adv_w = 80, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1101, .adv_w = 80, .box_w = 5, .box_h = 2, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1103, .adv_w = 80, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1105, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1109, .adv_w = 80, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1113, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1118, .adv_w = 80, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1123, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1129, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1134, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1139, .adv_w = 80, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1143, .adv_w = 80, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1146, .adv_w = 80, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1149, .adv_w = 80, .box_w = 1, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1151, .adv_w = 80, .box_w = 1, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1153, .adv_w = 80, .box_w = 2, .box_h = 9, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 1156, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1161, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1167, .adv_w = 80, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1173, .adv_w = 80, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 1174, .adv_w = 80, .box_w = 1, .box_h = 9, .ofs_x = 4, .ofs_y = -2},
    {.bitmap_index = 1176, .adv_w = 80, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1178, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1180, .adv_w = 80, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 1182, .adv_w = 80, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1189, .adv_w = 80, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1196, .adv_w = 80, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1203, .adv_w = 80, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1210, .adv_w = 80, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 1212, .adv_w = 80, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1219, .adv_w = 80, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 160, .range_length = 96, .glyph_id_start = 96,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 9472, .range_length = 160, .glyph_id_start = 192,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 3,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t term_font = {
#else
lv_font_t term_font = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 11,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if TERM_FONT*/

