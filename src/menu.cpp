#include "menu.hpp"
#include <clocale>
#include <cstdlib>
#include <langinfo.h>
extern "C" {
#include "extern/st_menu.h"
}

ST_MENU *menu;
ST_MENU_CONFIG config;
ST_MENU_ITEM _file[] = {
    {const_cast<char *>("~Q~uit"), 34, const_cast<char *>("Ctrl-q"), 0, 0, 0, NULL},
    {NULL, -1, NULL, 0, 0, 0, NULL}};
ST_MENU_ITEM _config[] = {
    {const_cast<char *>("~F~irst"), 1, NULL, 1, 0, 0, NULL},
    {const_cast<char *>("~S~econd"), 1, NULL, 2, 0, 0, NULL},
    {const_cast<char *>("~T~hird"), 1, NULL, 3, 0, 0, NULL},
    {const_cast<char *>("--"), -1, NULL, 0, 0, 0, NULL},
    {const_cast<char *>("Switch one"), 1, NULL, 0, 0, 0, NULL},
    {const_cast<char *>("Switch two"), 1, NULL, 0, 0, 0, NULL},
    {NULL, -1, NULL, 0, 0, 0, NULL}};

ST_MENU_ITEM menubar[] = {
    {const_cast<char *>("~F~ile"), 61, NULL, 0, 0, 0, _file},
    {const_cast<char *>("~C~onfig"), 62, NULL, 0, 0, 0, _config},
    {NULL, -1, NULL, 0, 0, 0, NULL}};

ST_CMDBAR *cmdbar;
ST_CMDBAR_ITEM bottombar[] = {
    {const_cast<char *>("Help"), false, 1, 1, 0},
    {const_cast<char *>("Menu"), false, 2, 2, 0},
    {const_cast<char *>("View"), false, 3, 3, 0},
    {const_cast<char *>("Edit"), false, 4, 4, 0},
    {const_cast<char *>("Copy"), false, 5, 5, 0},
    {const_cast<char *>("PullDn"), false, 9, 99, 0},
    {const_cast<char *>("Quit"), false, 10, 100, 0},
    {NULL, false, 0, 0, 0}};

void initialize_menu() {
    setlocale(LC_ALL, "");
    config.encoding = nl_langinfo(CODESET);
    config.language = NULL;
    config.force8bit = 1;

    st_menu_load_style(&config, ST_MENU_STYLE_VISION, 2, false, false);
    mousemask(BUTTON1_PRESSED | BUTTON1_RELEASED, NULL);
    mouseinterval(0);
    st_menu_set_desktop_window(stdscr);

    menu = st_menu_new_menubar(&config, menubar);
    st_menu_set_focus(menu, ST_MENU_FOCUS_ALT_MOUSE);
    st_menu_post(menu);

    cmdbar = st_cmdbar_new(&config, bottombar);
    st_cmdbar_post(cmdbar);

    atexit(kill_menu);
}

void kill_menu() {
    st_menu_unpost(menu, true);
    st_menu_free(menu);
}

void refresh_menu() {
    st_menu_post(menu);
    st_cmdbar_post(cmdbar);
    doupdate();
}

bool process_menu(int c, bool alt, MEVENT *mevent) {
    bool processed = st_menu_driver(menu, c, alt, mevent);
    doupdate();

    bool activated;
    ST_MENU_ITEM *item = st_menu_selected_item(&activated);
    if (processed and activated) {
        if (item->code == 34 || item->code == 100) {
            exit(0);
            return true;
        }
    } else if (processed) {
        ST_CMDBAR_ITEM *item = st_menu_selected_command(&activated);
        if (activated) {
            if (item->code == 100) {
                exit(0);
                return true;
            } else if (item->code == 99) {
                st_menu_set_focus(menu, ST_MENU_FOCUS_FULL);
                refresh_menu();
            } else {
                return true;
            }
        }
    }

    if (!processed and c == ST_MENU_ESCAPE) {
        st_menu_set_focus(menu, ST_MENU_FOCUS_ALT_MOUSE);
        refresh_menu();
        return true;
    }
    if (!processed and (c == KEY_MOUSE or c == KEY_F(10))) {
        st_menu_set_focus(menu, ST_MENU_FOCUS_ALT_MOUSE);
        refresh_menu();
        return true;
    }
    return false;
}