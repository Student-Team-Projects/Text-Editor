#include <cassert>
#include <ncurses.h>
#include <string>
#include <vector>

#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif

void initialize_ncurses() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();

    init_pair(1, COLOR_RED, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
}

void fill_row(int n_row, int color) {
    int COLS, ROWS;
    getmaxyx(stdscr, ROWS, COLS);
    std::string row(COLS, ' ');

    attron(COLOR_PAIR(color));
    mvprintw(n_row, 0, row.c_str());
    attroff(COLOR_PAIR(color));
}

const std::vector<std::string> topmenu_items = {"~F~ile", "~E~dit", "~W~indows", "~S~ettings"};
const std::vector<std::string> bottommenu_items = {"~Ctrl+N~ New", "~Ctrl+O~ Open", "~Ctrl+S~ Save", "~Ctrl+Q~ Quit"};

void draw_menu(const std::vector<std::string> &items, int COLS, int ROW, int line_begin = 2, int selected = -1) {
    fill_row(ROW, 1);

    int x = line_begin;
    for (size_t i = 0; i < items.size(); i += 1) {
        auto set_format = [i, selected]() {
            if (i == selected) {
                attron(COLOR_PAIR(4));
            } else {
                attron(COLOR_PAIR(2));
            }
        };
        auto set_shortcut_format = [i, selected]() {
            if (i == selected) {
                attron(COLOR_PAIR(3));
            } else {
                attron(COLOR_PAIR(1));
            }
        };
        set_format();

        mvaddch(ROW, x, ' ');
        x += 1;

        int m = 0;
        for (size_t j = 0; j < items[i].size(); j += 1) {
            if (items[i][j] == '~') {
                m ^= 1;
                if (m) {
                    set_shortcut_format();
                } else {
                    set_format();
                }
                continue;
            }
            mvaddch(ROW, x, items[i][j]);
            x += 1;
        }

        mvaddch(ROW, x, ' ');
        x += 1;
    }
}

void draw_topmenu(int selected) {
    int COLS, ROWS;
    getmaxyx(stdscr, ROWS, COLS);
    draw_menu(topmenu_items, COLS, 0, 2, selected);
}

void draw_bottommenu() {
    int COLS, ROWS;
    getmaxyx(stdscr, ROWS, COLS);
    draw_menu(bottommenu_items, COLS, ROWS - 1, 0);
}

int main() {
    initialize_ncurses();
    size_t selected = 0;

    while (true) {
        draw_topmenu(selected);
        draw_bottommenu();
        refresh();
        int key = getch();

        switch (key) {
        case KEY_LEFT:
            if (selected > 0) {
                selected -= 1;
            }
            break;
        case KEY_RIGHT:
            if (selected + 1 < topmenu_items.size()) {
                selected += 1;
            }
            break;
        case CTRL('q'):
            endwin();
            return 0;
        }
    }

    assert(false);
}