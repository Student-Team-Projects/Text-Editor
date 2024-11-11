#pragma once
#include <ncurses.h>

void initialize_menu();
void kill_menu();
void refresh_menu();
bool process_menu(int c, bool alt, MEVENT *mevent);