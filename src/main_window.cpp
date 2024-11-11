#include "main_window.hpp"
#include "ncurses_utils.hpp"
#include "utils.hpp"

MainWindow::MainWindow(WINDOW *win) : win(win) {
}

void MainWindow::draw() {
    box(win, 0, 0);
    fill_area(win, 1, getmaxx(win) - 2, 1, getmaxy(win) - 2, Attributes::WHITE_ON_BLUE);
    wrefresh(win);
}