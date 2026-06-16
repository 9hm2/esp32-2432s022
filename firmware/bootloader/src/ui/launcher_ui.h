// LVGL felhasználói felület a launcherhez (lista + kiválasztás).
#pragma once

#include "../sd_apps.h"

// Visszahívás, amikor a felhasználó kiválaszt egy appot a listából.
typedef void (*app_selected_cb_t)(const AppEntry &app);

// A főképernyő felépítése (cím + üres lista konténer). smartdisplay_init() után.
void launcher_ui_init();

// A lista feltöltése a talált appokkal. A kiválasztáskor 'cb' hívódik.
// Az 'apps' tömbnek a hívás után is élnie kell (a UI az indexet tárolja).
void launcher_ui_set_apps(const AppEntry *apps, size_t count, app_selected_cb_t cb);

// Egyszerű modális üzenet (cím + szöveg + OK). Hibákhoz / visszajelzéshez.
void launcher_ui_show_message(const char *title, const char *msg);
