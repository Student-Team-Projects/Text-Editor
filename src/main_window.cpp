#include "main_window.hpp"

MainWindow::MainWindow(WINDOW *win) : win(win) {
    box(win, 0, 0);
}

void MainWindow::draw() {
    wrefresh(win);
}