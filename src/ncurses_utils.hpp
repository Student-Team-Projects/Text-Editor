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
    static const attr_t RED_ON_WHITE = COLOR_PAIR(1),
                        BLACK_ON_WHITE = COLOR_PAIR(2),
                        RED_ON_GREEN = COLOR_PAIR(3),
                        BLACK_ON_GREEN = COLOR_PAIR(4),
                        WHITE_ON_BLUE = COLOR_PAIR(5),
                        REVERSE = A_REVERSE;
};
void set_format(WINDOW *win, attr_t format);
void reset_format(WINDOW *win);