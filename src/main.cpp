#include "bottom_row.hpp"
#include "main_window.hpp"
#include "ncurses_utils.hpp"
#include "timer.hpp"
#include "utils.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    initialize_ncurses();

    auto [n_rows, n_cols] = get_dimensions(stdscr);

    WINDOW *bottom_row_window = newwin(1, n_cols, n_rows - 1, 0);
    BottomRow bottom_row(bottom_row_window);

    WINDOW *timer_window = newwin(1, TIMER_SIZE, 0, n_cols - TIMER_SIZE);
    Timer timer(timer_window);

    WINDOW *main_window = newwin(n_rows - 2, n_cols, 1, 0);
    MainWindow main(main_window);

    refresh();
    bottom_row.draw();
    timer.draw();
    main.draw();

    while (1) {
        // bottom_row.draw(); don't need to, the only static element we will have :P
        timer.draw();
        main.draw();

        int ch = getch();
        if (ch == CTRL('q')) {
            break;
        }
    }
}