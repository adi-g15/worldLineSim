#include <iostream>

#include <thread>
#include <chrono>

#include "curses.h"

// #include "multi_term.hpp"
#include "single_term.hpp"

using namespace std;

int main(int, char const* []){

    single_term s("Aditya", "This is something");
    this_thread::sleep_for(3s);

#if 0
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);

    curs_set(FALSE);    //don't display the cursor

    int row, col;
    int bottom = 6;

    getmaxyx(stdscr, row, col);
    // cout<<row <<" * " << col <<endl;

    WINDOW* win1, * win2;
    win1 = newwin(row - bottom, col, 0, 0);
    win2 = newwin(bottom, col, row - bottom, 0);

    int count = 6;
    while( count-- ){
        wborder(win1, 0, 0, 0, 0, 0, 0, 0, 0);
        wrefresh(win1);

        this_thread::sleep_for(500ms);
    }


    wborder(win1, '-', '-', '-', '-', '-', '-', '-', '-');
    mvwaddstr(win1, 0, 0, "Win1");
    wrefresh(win1);

    this_thread::sleep_for(2s);

    mvwaddstr(win2, 0, 0, "Win2");
    wrefresh(win2);

    this_thread::sleep_for(1s);
    getch();    // this causes the output buffer to be flushed to the screen

    delwin(win1);
    delwin(win2);
    endwin();
#endif


    // single_term s;   

    // initscr();
    // temp();
    // getch();
    // endwin();

    // temp2();


    // WINDOW* win =  newwin(0,0,0,0);

    // // box(stdscr, '|', '-');
    // wvline(win, '+', 10);
    // box(win, '|','*');
    // wmove(win, 10,5);
    // waddstr(win, "Hi there");
    // wrefresh(win);
    // // mvaddstr(10, 5, "Hi there");
    // delwin(win);

    return 0;
}
