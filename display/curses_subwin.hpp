#pragma once

#include "curses.h"
#include "util/coord.hpp"

#include <memory>

/* @caution - DONT manipulate WINDOW* pointers directly
            If newwin() is needed, create a new class Window to handle memory allocation for that, currently it's not needed so leaving
 */
class SubWindow{
    typedef std::shared_ptr<SubWindow> SubWindow_Ptr;
public:
    bool enabled{false};
    void box(char32_t = ACS_VLINE, char32_t = ACS_HLINE);
    void refeshParent();
    void refesh();
    void disable(); // @brief -> RESETS  this->win to nullptr (All data used to construct is still stored, to enable at later stage)
    void enable(); // @brief -> ALLOCATES  this->win to nullptr (All data used to construct is still stored, to enable at later stage)
    void updateDimen(); // @brief - Updates contents of this->dimensions
    void mvnextLine();  // move to the next line (uses this->last_write_pos)

    // @brief - Wrappers around waddstr() and mvwaddstr()
    void addstr(std::string_view str);
    void addstr(int row, int col, std::string_view str);
    // @brief - Move to a new line, and add a string, there
    void nladdstr(std::string_view str);

    /**
     * @important @note - The pairs will be in the form of row*col
     * @brief - Uses stdscr as the parent screen
    */
    SubWindow(const std::pair<int,int>& dimensions, const std::pair<int,int>& start_coords):
        SubWindow(height, width, y_start, x_start)
    {
        this->enabled = true;
    }
    SubWindow(SubWindow_Ptr& parent_win, const std::pair<int,int>& dimensions, const std::pair<int,int>& start_coords):
        SubWindow(parent_win, height, width, y_start, x_start)
    {
        this->enabled = true;
    }


    SubWindow(int height, int width, int y_start, int x_start):
        height(height),
        width(width),
        y_start(y_start),
        x_start(x_start),
        win(subwin(stdscr, height, width, y_start, x_start))        
    {
        this->updateDimen();
        this->enabled = true;
    }
    SubWindow(SubWindow_Ptr& parent_win, int height, int width, int y_start, int x_start): 
        height(height),
        width(width),
        y_start(y_start),
        x_start(x_start),
        parent_win(parent_win),
        win(subwin(parent_win->win.get(), height, width, y_start, x_start))
    {
        this->updateDimen();
        this->enabled = true;
    }

    ~SubWindow(){
        // @note @future - for multi-threading, this next if MUST be atomic (for eg. two owners release the pointers simulateusly but for both this if may give false, that's the problem)
        if( win.use_count() == 1 ){	//  only we ourselves remain as its owner
            delwin(win.get());
            win.reset();	// not actually needed, but seems more explaining
        }
        if( parent_win.use_count() == 1 ){
            delwin(parent_win->win.get());
            parent_win.reset();	// not actually needed, but seems more explaining
        }
    }

private:
    const int height, width, y_start, x_start;  // data required for enable disable
    struct {
        int n_row;
        int n_col;
    }dimensions;
    struct {
        int n_row{0};
        int n_col{0};
    }last_write_pos;    // last locations to which we wrote anything
    std::shared_ptr<WINDOW> win;
    SubWindow_Ptr parent_win; // @note - this shared_ptr will hold NOTHING, when parent window is stdscr

    void moveCursor(int row, int col);

};

void SubWindow::box(char32_t vline, char32_t hline){
    if( this->win ){
        ::box(win.get(), vline, hline); // calling the one available in global scope
    }
    this->updateDimen();
}

void SubWindow::refesh(){
    wrefresh(this->win.get());
    this->updateDimen();
}

void SubWindow::refeshParent(){
    wrefresh(this->parent_win->win.get());
    this->updateDimen();
}

void SubWindow::disable(){
    if( ! this->enabled )  return;

    if(this->win.use_count() == 1){ // @caution @ignore - Read about shared_ptr::unique and shared_ptr::use_count(), these are not really accurate in multi-threading cases, where another thread has a reference to this shared_ptr
        delwin( this->win.get() );
    }
    this->win.reset();
    this->dimensions.n_col = 0;
    this->dimensions.n_row = 0;

    this->enabled = false;
}

void SubWindow::enable(){
    if( this->enabled )  return;
    
    if( !this->win ){
        if( !this->parent_win )
            this->win.reset( subwin(stdscr, height, width, y_start, x_start) );
        else
            this->win.reset( subwin(this->parent_win->win.get(), height, width, y_start, x_start) );
    }

    this->enabled = true;
    this->updateDimen();
}

void SubWindow::updateDimen(){
    // @note - May require mutex locks, to prevent multiple threads trying to wriet to the same thing
    getmaxyx( win.get(), dimensions.n_col, dimensions.n_row );
}

void SubWindow::addstr(std::string_view str){
    waddstr( this->win.get(), str.data() );
}

/**
 * @note - If either of row or col is negative, then it will be understood as max-the_value, for eg. row = -1, will be row = getmaxx(this->win.get())-1
*/
void SubWindow::addstr(int row, int col, std::string_view str){
    if( row < 0 )
        row = getmaxy( this->win.get() ) - row;

    if( col < 0 )
        col = getmaxx( this->win.get() ) - col;

    this->moveCursor(row, col);
    return this->addstr(str);
}

void SubWindow::moveCursor(int row, int col){
    wmove( this->win.get(), row, col );
    
    this->last_write_pos.n_row = row;
    this->last_write_pos.n_col = col;
}

    // move to the next line (NL), and add str
void SubWindow::nladdstr(std::string_view str){
    this->mvnextLine();
    this->addstr(str);
}

void SubWindow::mvnextLine(){
    this->moveCursor( this->last_write_pos.n_row + 1, this->last_write_pos.n_col );
}
