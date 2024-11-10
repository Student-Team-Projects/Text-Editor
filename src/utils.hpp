#pragma once
#include <ncurses.h>
#include <utility>

void fill_area(WINDOW *win, size_t x_from, size_t x_to, size_t y_from, size_t y_to, attr_t attr);
void fill_area(WINDOW *win, attr_t attr);
size_t get_ncols(WINDOW *win);
size_t get_nrows(WINDOW *win);
std::pair<size_t, size_t> get_dimensions(WINDOW *win);
