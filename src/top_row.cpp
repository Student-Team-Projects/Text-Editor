#include "top_row.hpp"
#include <string>
#include <vector>

const std::vector<std::string> menu_items = {"~F~ile", "~E~dit", "~W~indows", "~S~ettings"};

TopRow::TopRow(WINDOW *window) : win(window) {}

void TopRow::draw() {
    wrefresh(win);
}