

#include <cstdio>
#include <ncurses.h>

int main(){


    initscr();
    refresh();


    int w, h;
    getmaxyx(stdscr, h, w);     //size of screan
    
    int box_w = 40, box_h = 10;

    while (1) {

        WINDOW * win = newwin(box_h, box_w, (h-box_h)/2, (w-box_w)/2);
        auto b = box(win, 0, 0);
        addstr("AAA");
        box(win,0,0);
        mvwaddstr(win,3,3,"Welcome to ");
        mvwaddstr(win,5,3,"TURBO PASCHA, ඞ");
        wrefresh(win);

        char key = getch();
        wclear(win);
        if( key == 27) break;
    }
  
    endwin();
    return 0;
}

