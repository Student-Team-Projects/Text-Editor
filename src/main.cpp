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

void fill_row(size_t n_cols, size_t i_row, int color) {
    std::string row(n_cols, ' ');

    attron(COLOR_PAIR(color));
    mvaddnstr(i_row, 0, row.c_str(), n_cols);
    attroff(COLOR_PAIR(color));
}

const std::vector<std::string> topmenu_items = {"~F~ile", "~E~dit", "~W~indows", "~S~ettings"};
const std::vector<std::string> bottommenu_items = {"~Ctrl+N~ New", "~Ctrl+O~ Open", "~Ctrl+S~ Save", "~Ctrl+Q~ Quit"};

void draw_menu(const std::vector<std::string> &items, size_t n_cols, size_t i_row, size_t line_begin = 2, size_t selected = size_t(-1)) {
    fill_row(n_cols, i_row, 1);

    size_t x = line_begin;
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

        mvaddch(i_row, x, ' ');
        x += 1;

        bool isShortcut = 0;
        for (auto j : items[i]) {
            if (j == '~') {
                isShortcut ^= 1;
                if (isShortcut) {
                    set_shortcut_format();
                } else {
                    set_format();
                }
                continue;
            }
            mvaddch(i_row, x, j);
            x += 1;
        }

        mvaddch(i_row, x, ' ');
        x += 1;
    }
}

auto get_dimensions() {
    size_t n_cols, n_rows;
    getmaxyx(stdscr, n_rows, n_cols);
    return std::make_pair(n_cols, n_rows);
}

void draw_topmenu(size_t selected) {
    auto [n_cols, n_rows] = get_dimensions();
    draw_menu(topmenu_items, n_cols, 0, 2, selected);
}

void draw_bottommenu() {
    auto [n_cols, n_rows] = get_dimensions();
    draw_menu(bottommenu_items, n_cols, n_rows - 1, 0);
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
            if (selected > 0) selected -= 1;
            break;
        case KEY_RIGHT:
            if (selected + 1 < topmenu_items.size()) selected += 1;
            break;
        case CTRL('q'):
            endwin();
            return 0;
        }
    }

    assert(false);
}