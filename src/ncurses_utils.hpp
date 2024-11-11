#pragma once
#include <ncurses.h>

#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif

#ifndef ALT
#define ALT(c) ((c) & 0177)
#endif

void initialize_ncurses();
void exit_ncurses();
class Attributes {
public:
    static const attr_t RED_ON_WHITE = COLOR_PAIR(64),
                        BLACK_ON_WHITE = COLOR_PAIR(65),
                        RED_ON_GREEN = COLOR_PAIR(66),
                        BLACK_ON_GREEN = COLOR_PAIR(67),
                        WHITE_ON_BLUE = COLOR_PAIR(68);
};
void set_format(WINDOW *win, attr_t format);
void reset_format(WINDOW *win);