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

    if (c == 27) {
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
    WINDOW *timer_window = subwin(stdscr, 1, TIMER_SIZE, 0, n_cols - TIMER_SIZE);
    Timer timer(timer_window);

    WINDOW *main_window = subwin(stdscr, n_rows - 2, n_cols, 1, 0);
    MainWindow main(main_window);

    refresh();
    timer.draw();
    main.draw();

    while (1) {
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