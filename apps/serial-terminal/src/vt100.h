// Minimál, de valódi VT100/ANSI terminál-emuláció: karakterrács + kurzor +
// escape-szekvencia parser. A rendszer a rácsot egy egyedi LVGL rajzoló-objektum
// jeleníti meg (lásd main.cpp).
#pragma once

#include <Arduino.h>
#include <lvgl.h>

// Cella-attribútum bitek.
enum
{
    VT_BOLD = 1,
    VT_INVERSE = 2,
    VT_FG_DEF = 4, // alapértelmezett elotér (a paletta helyett)
    VT_BG_DEF = 8, // alapértelmezett háttér
};

struct VtCell
{
    uint8_t ch;    // ASCII karakter (' ' az üres)
    uint8_t fg;    // 256-szín paletta index (ha nem VT_FG_DEF)
    uint8_t bg;    // 256-szín paletta index (ha nem VT_BG_DEF)
    uint8_t flags; // VT_* bitek
};

// Max rács (a tájolásból adódó maximum bõven belefér).
static constexpr int VT_MAX_COLS = 60;
static constexpr int VT_MAX_ROWS = 40;

// Visszagörgetési (scrollback) puffer sorainak száma.
static constexpr int VT_SCROLLBACK = 120;

class Vt100
{
public:
    void init(int cols, int rows);
    void resize(int cols, int rows); // tartalmat törli, új méret
    void reset();                    // törlés + alaphelyzet

    void feed(uint8_t b);
    void feedStr(const char *s);

    bool dirty() const { return _dirty; }
    void clearDirty() { _dirty = false; }

    int cols() const { return _cols; }
    int rows() const { return _rows; }
    const VtCell &cell(int x, int y) const { return _cells[y * _cols + x]; }
    int curX() const { return _cx; }
    int curY() const { return _cy; }
    bool cursorVisible() const { return _cursorVisible; }

    // Megjelenítendo cella a görgetési pozíciót (scrollback) figyelembe véve.
    const VtCell &viewCell(int x, int vy) const;
    int scroll() const { return _scroll; }            // 0 = élo (alul)
    int scrollbackCount() const { return _sbCount; }
    void setScroll(int s);                             // abszolút görgetés
    void scrollBy(int lines);                          // relatív (>0 = vissza)
    void scrollToBottom() { setScroll(0); }

    // 256-szín paletta -> RGB (LVGL szín).
    static lv_color_t palette(uint8_t idx);
    static lv_color_t defaultFg();
    static lv_color_t defaultBg();

private:
    int _cols = 0, _rows = 0;
    int _cx = 0, _cy = 0;     // kurzor
    int _sx = 0, _sy = 0;     // mentett kurzor
    int _top = 0, _bot = 0;   // görgetési régió (sorok)
    bool _dirty = true;
    bool _cursorVisible = true;

    // Aktuális (SGR) attribútumok.
    uint8_t _fg = 0, _bg = 0;
    bool _fgDef = true, _bgDef = true;
    bool _bold = false, _inv = false;

    VtCell _cells[VT_MAX_COLS * VT_MAX_ROWS];

    // Scrollback gyuru-puffer.
    VtCell _sb[VT_SCROLLBACK * VT_MAX_COLS];
    int _sbHead = 0, _sbCount = 0, _scroll = 0;
    void pushScrollback(int row);
    const VtCell *sbLine(int i) const;

    // Parser-állapot.
    enum class St
    {
        Normal,
        Esc,
        Csi,
        Charset // ESC ( / ) utáni 1 karakter eldobása
    } _st = St::Normal;
    int _params[8];
    int _nparams = 0;
    bool _priv = false; // CSI '?'

    VtCell blank() const;
    void clearRow(int y);
    void copyRow(int dst, int src);
    void putChar(uint8_t ch);
    void lineFeed();
    void scrollUpRange(int top, int bot, int n);
    void scrollDownRange(int top, int bot, int n);

    void csiDispatch(uint8_t final);
    void applySgr();
    int param(int i, int def) const;
};
