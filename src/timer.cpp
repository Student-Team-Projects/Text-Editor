#include "timer.hpp"
#include "ncurses_utils.hpp"
#include <cassert>
#include <chrono>

Timer::Timer(WINDOW *win) : win(win) {
    set_format(win, Attributes::BLACK_ON_WHITE);
    assert(win != nullptr);
}

void Timer::draw() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_str = std::string(ctime(&now_time_t));

    mvwaddnstr(win, 0, 0, now_str.c_str(), now_str.size());
    wrefresh(win);
}