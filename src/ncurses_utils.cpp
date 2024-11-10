#include "ncurses_utils.hpp"
#include <cstdlib>

void initialize_ncurses() {

    initscr();            // ncurses internal initialization
    raw();                // disable line buffering
    noecho();             // disable echoing of characters
    timeout(500);         // getch will block for 500ms
    keypad(stdscr, TRUE); // enable function keys
    curs_set(0);          // hide cursor
    start_color();        // enable color
    atexit(exit_ncurses); // call exit_ncurses() at exit

    if (has_colors() == false) {
        printf("Your terminal does not support color\n");
        getch();
        exit(1);
    }

    init_pair(1, COLOR_RED, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    init_pair(5, COLOR_WHITE, COLOR_BLUE);
}

void exit_ncurses() { endwin(); }

void set_format(WINDOW *win, attr_t format) { wattron(win, format); }

void reset_format(WINDOW *win) { wattrset(win, A_NORMAL); }
