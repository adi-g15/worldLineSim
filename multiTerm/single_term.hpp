#pragma once


#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <future>
#include <thread>

#include "signal.h"
#include "curses.h"

void sig_handler(int SIGNAL){   // @warning -> It doesn't close the program itself, do that yourself (since way of doing so can be different according to preferences, for eg. i prefer using exceptions many a times)
    for (auto *win : this->_Windows)
    {
        if(win != nullptr)
        {
            delwin(win);
        }
    }

    endwin();        
}

/*
    Meant to be inherited from
*/
class single_term{  //initially will have 3 windows, a square one at bottom left, rectangular at bottom right, and a rectangular intoduction at top
    bool curses_initialized{ false };
    protected:
    std::string title;
    std::string description;    // @note - Not now (not enough space)

    std::vector< WINDOW* > _Windows;
    std::vector<std::pair< int, int >> _win_Dimensions;  //windows dimensions

    int _terminal_y;
    int _terminal_x;

    public:
    void printMsg(int win_index, const std::string& msg, int row = 0, int col = 0){
        if( win_index >= this->_Windows.size() ){
            // @future - After logger support is added, modify this
            // verse::LOGGER << "Window index passed is not available";
            return;
        }

        mvwaddstr(this->_Windows[win_index], row, col, msg.data());
        this->refreshWindow(win_index);
    }

    void clearAll(){
        wclear(stdscr);
    }

    void refreshAll(){
        // for( WINDOW* win : this->_Windows ){
        //     wrefresh(win);
        // }
        wrefresh(stdscr);
    }

    void refreshWindow(int win_index){
        wrefresh(this->_Windows[win_index]);
    }

    void checkResize(){
        // @future @me - Currently it will be managed inside the overloaded runInitTasks() member function, by the child class, though this function may contain a template like stuff for such a support, though it should not be used here
    }

    std::future<char> get_async_input(){
        return std::async(
            std::launch::async,
            [](){
                char ch;
                std::scanf("%c", &ch);

                return ch;
            }
        );
    }

    // @note - by default, curses_init will show an empty box
    void curses_init(){
        signal(SIGTERM, &sig_handler);   // @fix_me - 2nd argument's type isn't according to the declaration of signal() function

        #if defined(_IOSTREAM_) || defined(_GLIBCXX_IOSTREAM)
            std::ios_base::sync_with_stdio(false);  // only this maybe enough, depending on what is used inside single_term::get_async_input
            std::cin.tie(0);
            std::cout.tie(0);
        #endif

        if( this->curses_initialized ){
            return;
        }

        initscr();
        curs_set(FALSE);    // cursor won't show
        keypad(stdscr, TRUE);
        nonl(); // tells cursor not to do NL -> CR/NL on output
        noecho();
        cbreak();   // no wait for '\n', one char at a time

        this->curses_initialized = true;
        getmaxyx(stdscr, this->_terminal_y, this->_terminal_x);

        // an empty box
        box( stdscr, ACS_VLINE, ACS_HLINE );
        wrefresh( stdscr );
    }
    virtual void runInitTasks() = 0;

    //currently number of windows is fixed, since this is not the primary goal, this will be as such for some time maybe
    single_term() = default;
    single_term(std::string_view title,std::string_view desc) : title(title), description(desc){}
    ~single_term();
};

    single_term::~single_term(){
        if( !curses_initialized ){
            return;
        }

        for( WINDOW* win : _Windows ){
            delwin(win);
        }

        endwin();
    }

