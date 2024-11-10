#pragma once
#include <ncurses.h>

class MainWindow {
public:
    MainWindow(WINDOW *win);
    void draw();

private:
    WINDOW *win;
};