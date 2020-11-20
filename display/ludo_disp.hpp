    // @note - This is the new display to be used

#pragma once

#include "forward_decl.hpp"
#include "curses_subwin.hpp"

#include "curses.h"

#include <array>
#include <signal.h>
#include <memory>

class Display
{
    typedef int32_t dimen_t;
    typedef std::shared_ptr<SubWindow> SubWindow_Ptr;
private:
    static void sig_handler(int signal);
    std::array<SubWindow_Ptr, 4> sub_windows;
    SubWindow_Ptr titleBar{ nullptr },
             * boardArea{ nullptr },
             * scoreArea{ nullptr },
             * dieArea{ nullptr };

    SubWindow_Ptr ludoBoard{ nullptr };  // subwindow of `boardArea`
    dimen_t y_term_dimen, x_term_dimen;

public:
    static bool curses_initiated;

    static void endCurses();

    Display();
    ~Display();
};

Display::Display()
{
    initscr();
    signal( SIGINT, Display::sig_handler );

    getmaxyx( stdscr, this->y_term_dimen, this->x_term_dimen );
    this->sub_windows[0].reset(new SubWindow(3, 0, 0, 0));
    this->sub_windows[1] = subwin(stdscr, 0, x_term_dimen*0.75f, 3, 0);
    this->sub_windows[2] = subwin(stdscr, (y_term_dimen - 3) * 0.8f, 0, 3, x_term_dimen * 0.75f);
    this->sub_windows[3] = subwin(stdscr, 0, 0, 3 + (y_term_dimen - 3) * 0.8f, x_term_dimen * 0.75f);

    this->titleBar = sub_windows[0];
    this->boardArea = sub_windows[1];
    this->scoreArea = sub_windows[2];
    this->dieArea = sub_windows[3];

    // box( dieArea, ACS_LANTERN, ACS_HLINE );
    for (auto *win : this->sub_windows)
    {
        if( win ){
            box( win, ACS_VLINE, ACS_HLINE );
        }
    }

    refresh();
}

Display::~Display()
{
    if( ! Display::curses_initiated ){
        Display::endCurses();
    }
}

void Display::endCurses(){
    endwin();
    Display::curses_initiated = false;
}

void Display::sig_handler(int signal){
    Display::endCurses();
}
