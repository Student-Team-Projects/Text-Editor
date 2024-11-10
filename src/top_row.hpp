#pragma once
#include <ncurses.h>

class TopRow {
public:
    TopRow(WINDOW *win);
    void draw();

private:
    WINDOW *win;
};