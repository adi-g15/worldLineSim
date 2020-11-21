#pragma once

#include "curses.h"
#include "util/coord.hpp"

#include <memory>

enum class position{    // only horizontal positions for now
    LEFT,
    MIDDLE,
    RIGHT,
    AT_CURS // don't change, just print AT the cursor position
};

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
    int getmax_x();
    int getmax_y();
    void updateDimen(); // @brief - Updates contents of this->dimensions
    void mvnextLine();  // move to the next line (uses last_write_pos)

    /**
     * @brief - Wrappers around waddstr() and mvwaddstr()
     * @params - str - String,
     *           position - Position to print at (LEFT(AT_CURS), MIDDLE, RIGHT)
     *           row - Y dimension to write at
     *           col - X dimension to write at
    */
    void addstr(std::string_view str, position = position::AT_CURS);
    void addstr(int row, int col, std::string_view str);
    // @brief - Move to a new line, and add a string, there
    void nladdstr(std::string_view str, position = position::AT_CURS);
    void addch(char ch);

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
    bool boxed{false};  // @caution - Currently no function is available that resets this to false (though, since it's not required in our case, leaving)
    std::shared_ptr<WINDOW> win;
    SubWindow_Ptr parent_win; // @note - this shared_ptr will hold NOTHING, when parent window is stdscr

    void moveCursor(int row, int col);

};
