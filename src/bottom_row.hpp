#pragma once
#include <ncurses.h>

class BottomRow {
public:
    BottomRow(WINDOW *win);
    void draw();

private:
    WINDOW *win;
};