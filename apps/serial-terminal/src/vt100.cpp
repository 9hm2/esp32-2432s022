#include "vt100.h"

// --- Színek ----------------------------------------------------------------

// Szabványos 16 ANSI szín (xterm értékek).
static const uint8_t ANSI16[16][3] = {
    {0, 0, 0},     {205, 0, 0},   {0, 205, 0},   {205, 205, 0},
    {0, 0, 238},   {205, 0, 205}, {0, 205, 205}, {229, 229, 229},
    {127, 127, 127}, {255, 0, 0}, {0, 255, 0},   {255, 255, 0},
    {92, 92, 255}, {255, 0, 255}, {0, 255, 255}, {255, 255, 255},
};

lv_color_t Vt100::palette(uint8_t idx)
{
    if (idx < 16)
        return lv_color_make(ANSI16[idx][0], ANSI16[idx][1], ANSI16[idx][2]);
    if (idx < 232)
    {
        int i = idx - 16;
        static const uint8_t lv[6] = {0, 95, 135, 175, 215, 255};
        return lv_color_make(lv[(i / 36) % 6], lv[(i / 6) % 6], lv[i % 6]);
    }
    uint8_t g = 8 + (idx - 232) * 10; // 232..255 szürkeárnyalat
    return lv_color_make(g, g, g);
}

lv_color_t Vt100::defaultFg() { return lv_color_hex(0xC8F0C8); }
lv_color_t Vt100::defaultBg() { return lv_color_hex(0x000000); }

// --- Rács alapmuveletek ----------------------------------------------------

VtCell Vt100::blank() const
{
    VtCell c;
    c.ch = ' ';
    c.fg = 0;
    c.bg = _bg; // "background color erase": az aktuális háttérrel törlünk
    c.flags = VT_FG_DEF | (_bgDef ? VT_BG_DEF : 0);
    return c;
}

void Vt100::clearRow(int y)
{
    VtCell b = blank();
    for (int x = 0; x < _cols; x++)
        _cells[y * _cols + x] = b;
}

void Vt100::copyRow(int dst, int src)
{
    memcpy(&_cells[dst * _cols], &_cells[src * _cols], _cols * sizeof(VtCell));
}

void Vt100::init(int cols, int rows) { resize(cols, rows); }

void Vt100::resize(int cols, int rows)
{
    _cols = constrain(cols, 1, VT_MAX_COLS);
    _rows = constrain(rows, 1, VT_MAX_ROWS);
    _top = 0;
    _bot = _rows - 1;
    _cx = _cy = 0;
    reset();
}

void Vt100::reset()
{
    _fgDef = _bgDef = true;
    _fg = _bg = 0;
    _bold = _inv = false;
    _cursorVisible = true;
    _st = St::Normal;
    for (int y = 0; y < _rows; y++)
        clearRow(y);
    _cx = _cy = 0;
    _top = 0;
    _bot = _rows - 1;
    _sbHead = _sbCount = _scroll = 0;
    _dirty = true;
}

// --- Scrollback ------------------------------------------------------------

const VtCell *Vt100::sbLine(int i) const
{
    return &_sb[((_sbHead + i) % VT_SCROLLBACK) * VT_MAX_COLS];
}

void Vt100::pushScrollback(int row)
{
    int slot = (_sbHead + _sbCount) % VT_SCROLLBACK;
    memcpy(&_sb[slot * VT_MAX_COLS], &_cells[row * _cols], _cols * sizeof(VtCell));
    if (_sbCount < VT_SCROLLBACK)
        _sbCount++;
    else
        _sbHead = (_sbHead + 1) % VT_SCROLLBACK; // legrégebbi felülírása
    if (_scroll > 0 && _scroll < _sbCount)
        _scroll++; // a nézet a régi soroknál marad (scroll-lock)
}

const VtCell &Vt100::viewCell(int x, int vy) const
{
    int v = (_sbCount - _scroll) + vy;
    if (v < _sbCount)
        return sbLine(v)[x]; // scrollback
    int sr = v - _sbCount;
    if (sr < 0) sr = 0;
    if (sr >= _rows) sr = _rows - 1;
    return _cells[sr * _cols + x];
}

void Vt100::setScroll(int s)
{
    _scroll = constrain(s, 0, _sbCount);
    _dirty = true;
}

void Vt100::scrollBy(int lines) { setScroll(_scroll + lines); }

void Vt100::scrollUpRange(int top, int bot, int n)
{
    if (n <= 0)
        return;
    // A képernyo tetejérol kigördülo sorokat a scrollbackbe mentjük.
    if (top == 0)
        for (int k = 0; k < n && k <= bot; k++)
            pushScrollback(k);
    for (int y = top; y + n <= bot; y++)
        copyRow(y, y + n);
    for (int y = bot - n + 1; y <= bot; y++)
        if (y >= 0 && y < _rows)
            clearRow(y);
    _dirty = true;
}

void Vt100::scrollDownRange(int top, int bot, int n)
{
    if (n <= 0)
        return;
    for (int y = bot; y - n >= top; y--)
        copyRow(y, y - n);
    for (int y = top; y < top + n; y++)
        if (y >= 0 && y < _rows)
            clearRow(y);
    _dirty = true;
}

void Vt100::lineFeed()
{
    if (_cy >= _bot)
        scrollUpRange(_top, _bot, 1);
    else if (_cy < _rows - 1)
        _cy++;
}

void Vt100::putChar(uint8_t ch)
{
    if (_cx >= _cols)
    {
        _cx = 0;
        lineFeed();
    }
    VtCell &c = _cells[_cy * _cols + _cx];
    c.ch = ch;
    c.fg = _fg;
    c.bg = _bg;
    c.flags = (_bold ? VT_BOLD : 0) | (_inv ? VT_INVERSE : 0) |
              (_fgDef ? VT_FG_DEF : 0) | (_bgDef ? VT_BG_DEF : 0);
    _cx++;
    _dirty = true;
}

// --- Parser ----------------------------------------------------------------

int Vt100::param(int i, int def) const
{
    // Üres vagy 0 paraméter -> az adott parancs alapértéke (bevett ANSI szokás).
    if (i < _nparams && _params[i] != 0)
        return _params[i];
    return def;
}

void Vt100::feedStr(const char *s)
{
    while (s && *s)
        feed((uint8_t)*s++);
}

void Vt100::feed(uint8_t b)
{
    switch (_st)
    {
    case St::Normal:
        if (b == 0x1B) { _st = St::Esc; return; }
        if (b == '\n') { lineFeed(); _dirty = true; return; }
        if (b == '\r') { _cx = 0; _dirty = true; return; }
        if (b == '\b') { if (_cx > 0) _cx--; _dirty = true; return; }
        if (b == '\t') { _cx = (_cx + 8) & ~7; if (_cx >= _cols) _cx = _cols - 1; return; }
        if (b == 0x07) return; // BEL
        if (b >= 0x20 && b < 0x7F) putChar(b);
        return;

    case St::Esc:
        if (b == '[')
        {
            _st = St::Csi;
            _nparams = 0;
            _priv = false;
            for (int i = 0; i < 8; i++)
                _params[i] = 0;
            return;
        }
        if (b == '(' || b == ')') { _st = St::Charset; return; }
        if (b == 'c') { reset(); _st = St::Normal; return; } // RIS
        if (b == 'D') { lineFeed(); _st = St::Normal; return; }            // IND
        if (b == 'M') { if (_cy <= _top) scrollDownRange(_top, _bot, 1); else _cy--;
                        _st = St::Normal; return; }                          // RI
        if (b == 'E') { _cx = 0; lineFeed(); _st = St::Normal; return; }    // NEL
        _st = St::Normal;
        return;

    case St::Charset:
        _st = St::Normal; // a karakterkészlet-jelölést eldobjuk
        return;

    case St::Csi:
        if (b == '?') { _priv = true; return; }
        if (b >= '0' && b <= '9')
        {
            if (_nparams == 0) _nparams = 1;
            _params[_nparams - 1] = _params[_nparams - 1] * 10 + (b - '0');
            return;
        }
        if (b == ';')
        {
            if (_nparams == 0) _nparams = 1;
            if (_nparams < 8) _params[_nparams++] = 0;
            return;
        }
        if (b >= 0x40 && b <= 0x7E)
        {
            csiDispatch(b);
            _st = St::Normal;
            return;
        }
        return; // egyéb köztes byte
    }
}

void Vt100::applySgr()
{
    if (_nparams == 0)
    {
        _fgDef = _bgDef = true;
        _bold = _inv = false;
        return;
    }
    for (int i = 0; i < _nparams; i++)
    {
        int p = _params[i];
        if (p == 0) { _fgDef = _bgDef = true; _bold = _inv = false; }
        else if (p == 1) _bold = true;
        else if (p == 22) _bold = false;
        else if (p == 7) _inv = true;
        else if (p == 27) _inv = false;
        else if (p >= 30 && p <= 37) { _fg = p - 30; _fgDef = false; }
        else if (p == 39) _fgDef = true;
        else if (p >= 40 && p <= 47) { _bg = p - 40; _bgDef = false; }
        else if (p == 49) _bgDef = true;
        else if (p >= 90 && p <= 97) { _fg = p - 90 + 8; _fgDef = false; }
        else if (p >= 100 && p <= 107) { _bg = p - 100 + 8; _bgDef = false; }
        else if (p == 38 || p == 48)
        {
            bool isFg = (p == 38);
            if (i + 2 < _nparams && _params[i + 1] == 5)
            {
                uint8_t idx = (uint8_t)_params[i + 2];
                if (isFg) { _fg = idx; _fgDef = false; } else { _bg = idx; _bgDef = false; }
                i += 2;
            }
            else if (i + 4 < _nparams && _params[i + 1] == 2)
            {
                // truecolor -> közelíto 6x6x6 kockára
                int r = _params[i + 2], g = _params[i + 3], bl = _params[i + 4];
                uint8_t idx = 16 + 36 * (r * 5 / 255) + 6 * (g * 5 / 255) + (bl * 5 / 255);
                if (isFg) { _fg = idx; _fgDef = false; } else { _bg = idx; _bgDef = false; }
                i += 4;
            }
        }
    }
}

void Vt100::csiDispatch(uint8_t f)
{
    switch (f)
    {
    case 'A': _cy = max(0, _cy - param(0, 1)); break;
    case 'B': _cy = min(_rows - 1, _cy + param(0, 1)); break;
    case 'C': _cx = min(_cols - 1, _cx + param(0, 1)); break;
    case 'D': _cx = max(0, _cx - param(0, 1)); break;
    case 'E': _cx = 0; _cy = min(_rows - 1, _cy + param(0, 1)); break;
    case 'F': _cx = 0; _cy = max(0, _cy - param(0, 1)); break;
    case 'G':
    case '`': _cx = constrain(param(0, 1) - 1, 0, _cols - 1); break;
    case 'd': _cy = constrain(param(0, 1) - 1, 0, _rows - 1); break;
    case 'H':
    case 'f':
        _cy = constrain(param(0, 1) - 1, 0, _rows - 1);
        _cx = constrain(param(1, 1) - 1, 0, _cols - 1);
        break;
    case 'J':
    {
        int m = param(0, 0);
        if (m == 0) { for (int x = _cx; x < _cols; x++) _cells[_cy * _cols + x] = blank();
                      for (int y = _cy + 1; y < _rows; y++) clearRow(y); }
        else if (m == 1) { for (int y = 0; y < _cy; y++) clearRow(y);
                           for (int x = 0; x <= _cx && x < _cols; x++) _cells[_cy * _cols + x] = blank(); }
        else { for (int y = 0; y < _rows; y++) clearRow(y); }
        _dirty = true;
        break;
    }
    case 'K':
    {
        int m = param(0, 0);
        int x0 = (m == 1) ? 0 : _cx;
        int x1 = (m == 0) ? _cols - 1 : (m == 1 ? _cx : _cols - 1);
        for (int x = x0; x <= x1 && x < _cols; x++) _cells[_cy * _cols + x] = blank();
        _dirty = true;
        break;
    }
    case 'L': scrollDownRange(_cy, _bot, param(0, 1)); break; // sor beszúrás
    case 'M': scrollUpRange(_cy, _bot, param(0, 1)); break;   // sor törlés
    case 'S': scrollUpRange(_top, _bot, param(0, 1)); break;
    case 'T': scrollDownRange(_top, _bot, param(0, 1)); break;
    case 'P': // karakter törlés (balra húz)
    {
        int n = min(param(0, 1), _cols - _cx);
        for (int x = _cx; x < _cols; x++)
            _cells[_cy * _cols + x] = (x + n < _cols) ? _cells[_cy * _cols + x + n] : blank();
        _dirty = true;
        break;
    }
    case '@': // karakter beszúrás (jobbra tol)
    {
        int n = min(param(0, 1), _cols - _cx);
        for (int x = _cols - 1; x >= _cx; x--)
            _cells[_cy * _cols + x] = (x - n >= _cx) ? _cells[_cy * _cols + x - n] : blank();
        _dirty = true;
        break;
    }
    case 'X': // karakterek törlése helyben
    {
        int n = min(param(0, 1), _cols - _cx);
        for (int x = _cx; x < _cx + n; x++) _cells[_cy * _cols + x] = blank();
        _dirty = true;
        break;
    }
    case 'm': applySgr(); break;
    case 'r':
        _top = constrain(param(0, 1) - 1, 0, _rows - 1);
        _bot = constrain(param(1, _rows) - 1, _top, _rows - 1);
        _cx = 0; _cy = _top;
        break;
    case 's': _sx = _cx; _sy = _cy; break;
    case 'u': _cx = _sx; _cy = _sy; break;
    case 'h':
    case 'l':
        if (_priv && param(0, 0) == 25) _cursorVisible = (f == 'h');
        break;
    default: break;
    }
    _dirty = true;
}
