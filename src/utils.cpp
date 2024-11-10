#include "utils.hpp"

void fill_area(WINDOW *win, size_t x_from, size_t x_to, size_t y_from, size_t y_to, attr_t attr) {
    for (size_t y = y_from; y <= y_to; y += 1) {
        for (size_t x = x_from; x <= x_to; x += 1) {
            mvwaddch(win, y, x, ' ' | attr);
        }
    }
}

void fill_area(WINDOW *win, attr_t attr) {
    auto [n_rows, n_cols] = get_dimensions(win);
    fill_area(win, 0, n_cols - 1, 0, n_rows - 1, attr);
}

size_t get_ncols(WINDOW *win) {
    int n_cols;
    [[maybe_unused]] int n_rows;
    getmaxyx(win, n_rows, n_cols);
    return n_cols;
}

size_t get_nrows(WINDOW *win) {
    [[maybe_unused]] int n_cols;
    int n_rows;
    getmaxyx(win, n_rows, n_cols);
    return n_rows;
}

std::pair<size_t, size_t> get_dimensions(WINDOW *win) {
    int n_cols, n_rows;
    getmaxyx(win, n_rows, n_cols);
    return std::make_pair(n_rows, n_cols);
}