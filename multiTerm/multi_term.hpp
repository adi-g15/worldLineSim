#pragma once

#include "curses.h"
#include <vector>

// @todo - Make the display responsive

class single_term{
private:
    std::vector< WINDOW* > windows;

    int term_height;
    int term_width;

public:
    single_term();
    ~single_term();
};

#include <thread>
#include <chrono>
void temp2(void){
    std::vector<WINDOW*> mWindows;

    mWindows.resize(3);    // Separate the screen to three windows
    initscr();

    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    noecho();    // No echo for the key pressed

    curs_set(0);    // No cursor show

    int mGameBoardWidth, mGameBoardHeight, mScreenWidth, mScreenHeight, mInstructionWidth=18, mInformationHeight =6 ;
    // Get screen and board parameters
    getmaxyx(stdscr, mScreenHeight, mScreenWidth);
    mGameBoardWidth = mScreenWidth - mInstructionWidth;
    mGameBoardHeight = mScreenHeight - mInformationHeight;

    // createInformationBoard();
    int startY = 0;
    int startX = 0;
    mWindows[0] = newwin(mInformationHeight, mScreenWidth, startY, startX);

    // createGameBoard();
    startY = mInformationHeight;
    startX = 0;
    mWindows[1] = newwin(mScreenHeight - mInformationHeight, mScreenWidth - mInstructionWidth, startY, startX);


    // createInstructionBoard();
    startY = mInformationHeight;
    startX = mScreenWidth - mInstructionWidth;
    mWindows[2] = newwin(mScreenHeight - mInformationHeight, mInstructionWidth, startY, startX);


    using namespace std;
    char s[45]="Hit here";
    mvaddstr(50,10,s);

    box(mWindows[2], '+', '-');

    refresh();
    // wrefresh(mWindows[1]);

    getch();
    getch();
    getch();

    this_thread::sleep_for(5s);


    for( int i = 0; i < mWindows.size(); i++ ){
        delwin(mWindows[i]);
    }
    endwin();

}

void temp(void){
    int BoardWidth=60, BoardHeight=20;
    WINDOW* menu;
    int width = BoardWidth * 0.5;
    int height = BoardHeight * 0.5;
    int startX = BoardWidth * 0.25;
    int startY = BoardHeight * 0.25 + 5;

    menu = newwin(height, width, startY, startX);
    box(menu, '-', '|');
    std::vector<std::string> menuItems = { "Restart", "Quit" };

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(20);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while( true ){
        key = getch();
        switch( key ){
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index--;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if( key == ' ' || key == 10 ){
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    // if( index == 0 ){
    //     return true;
    // } else{
    //     return false;
    // }

}

single_term::single_term(): windows(3){
    initscr();  //initialise the curses library
    getmaxyx(stdscr, term_width, term_height);

    // for now it will have three windows
    // windows.push_back(newwin());
    windows.push_back(newwin(45,20,2,4));

    /*
    LEARNT - initscr() basically sets up the routines to use current terminal characteristics, as well as allocate space for stdscr and curscr
    */
    keypad(stdscr, true);   // enable keyboard mapping, for arrow and function keys
    nonl(); // no newlines or control feeds in curses
    cbreak();   // take chars one at a time, don't wait for '\n' to be pressed
    noecho();   //when echo is enabled, getch() actually itself calls addch() to output entered character to screen

    // @note - scrollok() and leaveok() are somethings i may use in some other programs later

    int num = 0;

    // box(stdscr, ACS_VLINE, ACS_HLINE);

    temp();

#if 0
    box(windows[0], ACS_VLINE, ACS_HLINE);
    mvaddstr(0,0,"There");
    mvaddstr(4,40,"Hi there");
    wrefresh(windows[0]);
    refresh();
#endif

    // while (true)
    // {
    //     int c = getch();
    //     attrset(COLOR_PAIR( num%8 ));
    //     // COLOR_PAIR( num%8 );
    //     addch(c);
    //     getmaxyx(stdscr, term_width, term_height);
    //     // resize_term(term_width, term_height);
    //     num++;

    //     if(c == 'q') break;

    // }

    getch();

}

single_term::~single_term(){
    endwin();
}
