#include "bottom_row.hpp"
#include "main_window.hpp"
#include "menu.hpp"
#include "ncurses_utils.hpp"
#include "timer.hpp"
#include "utils.hpp"

/*
 * Read event. When event is mouse event, read mouse data
 */
static inline int
get_event(MEVENT *mevent, bool *alt) {
    bool first_event = true;
    int c;

    *alt = false;

repeat:

    c = getch();

    /*
     * Read mouse event when it is possible. Do it now, before st_meny_driver call,
     * as protection to unwanted multiple call of getmouse function. For one mouse
     * event, it returns data only once time.
     */
    if (c == KEY_MOUSE) {
        int ok = getmouse(mevent);

        if (ok != OK)
            goto repeat;
    }

    if (c == 27) { // escape character
        if (first_event) {
            first_event = false;
            goto repeat;
        }
    }

    *alt = !first_event;

    return c;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    initialize_ncurses();
    initialize_menu();

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

        MEVENT mevent;
        bool alt;
        int c = get_event(&mevent, &alt);
        bool processed = process_menu(c, alt, &mevent);
        if (!processed) {
            if (c == CTRL('q')) {
                break;
            }
        }
    }
}