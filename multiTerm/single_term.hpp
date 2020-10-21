#include "curses.h"

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <utility>

/*
    Meant to be inherited from
*/

class single_term{  //initially will have 3 windows, a square one at bottom left, rectangular at bottom right, and a rectangular intoduction at top
    private:
    std::string title;
    std::string description;

    std::vector< WINDOW* > _Windows;

    int _terminal_height;
    int _terminal_width;

    std::vector<std::pair< int, int >> _win_Dimensions;  //windows dimensions
    public:
    void printMsg(int win_index, const std::string& msg, int row = 0, int col = 0){
        if( win_index >= this->_Windows.size() ){
            std::clog << "Window index passed is not available";
            return;
        }

        mvwaddstr(this->_Windows[win_index], row, col, msg.data());
        this->refreshWindow(win_index);
    }

    void refreshAll(){
        for( WINDOW* win : this->_Windows ){
            wrefresh(win);
        }
    }

    void refreshWindow(int win_index){
        wrefresh(this->_Windows[win_index]);
    }

    void checkResize(){
        // @todo
    }

    virtual void init() = 0;

    //currently number of windows is fixed, since this is not the primary goal, this will be as such for some time maybe
    single_term(std::string_view, std::string_view);
    ~single_term();
};

single_term::single_term(std::string_view title, std::string_view desc) : title(title), description(desc){
    initscr();

    noecho();
    cbreak();

    getmaxyx(stdscr, this->_terminal_height, this->_terminal_width);
    this->_Windows.reserve(3);
    this->_win_Dimensions.reserve(3);

    this->_win_Dimensions[0] = { 0.2f * _terminal_height, _terminal_width };
    this->_win_Dimensions[1] = { 0.8f * _terminal_height, 0.8f * _terminal_width };
    this->_win_Dimensions[2] = { 0.8f * _terminal_height, 0.2f * _terminal_width };


    this->_Windows.push_back(newwin(_win_Dimensions[0].first, _win_Dimensions[0].second, 0, 0));
    this->_Windows.push_back(newwin(_win_Dimensions[1].first, _win_Dimensions[1].second, _terminal_height - _win_Dimensions[1].first, 0));
    this->_Windows.push_back(newwin(_win_Dimensions[2].first, _win_Dimensions[2].second, _terminal_height - _win_Dimensions[2].first, _terminal_width - _win_Dimensions[2].second));

    wborder(_Windows[0], 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(_Windows[0]);
    wborder(_Windows[1], 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(_Windows[1]);
    wborder(_Windows[2], 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(_Windows[2]);


    this->init();
}

single_term::~single_term(){
    for( WINDOW* win : _Windows ){
        delwin(win);
    }

    endwin();
}
