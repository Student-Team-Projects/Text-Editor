#pragma once
#include <ncurses.h>

const int TIMER_SIZE = 19;
class Timer {
public:
    Timer(WINDOW *win);
    void draw();

private:
    WINDOW *win;
};