#include "bottom_row.hpp"
#include "ncurses_utils.hpp"
#include "utils.hpp"
#include <cassert>
#include <string>
#include <vector>

const std::vector<std::string> menu_items = {"~Ctrl+N~ New", "~Ctrl+O~ Open", "~Ctrl+S~ Save", "~Ctrl+Q~ Quit"};

BottomRow::BottomRow(WINDOW *win) : win(win) {
    assert(win != nullptr);

    fill_area(win, Attributes::BLACK_ON_WHITE);

    size_t x = 0;
    for (size_t i = 0; i < menu_items.size(); i += 1) {
        set_format(win, Attributes::BLACK_ON_WHITE);

        mvwaddch(win, 0, x, ' ');
        x += 1;

        bool isShortcut = 0;
        for (auto j : menu_items[i]) {
            if (j == '~') {
                isShortcut ^= 1;
                if (isShortcut) {
                    set_format(win, Attributes::RED_ON_WHITE);
                } else {
                    set_format(win, Attributes::BLACK_ON_WHITE);
                }
                continue;
            }
            mvwaddch(win, 0, x, j);
            x += 1;
        }
        mvwaddch(win, 0, x, ' ');
        x += 1;
    }
}

void BottomRow::draw() {
    wrefresh(win);
}