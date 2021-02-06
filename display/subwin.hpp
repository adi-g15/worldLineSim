#pragma once

class SubWindow {};

#if 0
#include <iostream>
#include "curses.h"
#include "util/coord.hpp"

#include <memory>
#include <string>	// for std::to_string

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
	typedef std::shared_ptr<SubWindow> SubWindow_Ptr;   // NO OWNERSHIP for the parent will be held here
public:
	bool enabled{ false };
	void box(chtype = ' ', chtype = ' ');
	void refreshParent();
	void refresh();
	bool isEnabled();
	void disable(); // @brief -> RESETS  this->win to nullptr (All data used to construct is still stored, to enable at later stage)
	void enable(); // @brief -> ALLOCATES  this->win to nullptr (All data used to construct is still stored, to enable at later stage)
	int getmax_x();
	int getmax_y();
	void updateDimen(); // @brief - Updates contents of this->dimensions
	void newline();  // move to the next line (uses last_write_pos)
	void moveCursor(int row, int col);
	void hline(chtype = ACS_HLINE);   // @brief -> Draws a horizontal line on NEXT LINE
	void hline(int y, int x, char, int len);   // not defined now, not needed

	/**
	 * @brief - Wrappers around waddstr() and mvwaddstr()
	 * @params - str - String,
	 *           position - Position to print at (LEFT(AT_CURS), MIDDLE, RIGHT)
	 *           row - Y dimension to write at
	 *           col - X dimension to write at
	*/
	void addstr(std::string_view str, position = position::AT_CURS);
	void addstr(int row, int col, std::string_view str);
	// @future - Later implement the wrapper to mvwprintw() too, since it provides the formatted output
	// @brief - Move to a new line, and add a string, there
	void nladdstr(std::string_view str, position = position::AT_CURS);

	void addch(char ch);

	void printf(const char*);

	// definition of this templated function is REQUIRED here, so that the particular function can be formed by any file that includes curses_subwin.hpp
	template<typename T, typename... Types>
	void printf(const char* format, T first_val, Types... next_vals){
		for( int i = 0; format[i] != '\0'; ++i ){
			if( format[i] == '%' ){
				if( std::is_integral<T>::value ){
					this->addstr(std::to_string(first_val));
				} else if( std::is_same<T, char>::value ){
					this->addch(first_val);
				} else{
					// else ignore any other type
				}
				return this->printf(format + i + 1, next_vals...);

				// #if std::is_integral<T>::value
				// #elif std::is_same<T, char>::value
				//     this->addch(first_val);
				// #elif std::is_same<T, const char*>::value
				//     this->addstr(first_val);
				// #elif std::is_same<T, std::string>::value || std::is_same<T, std::string_view>::value
				//     this->addstr(first_val.data());
				// #else
				// #endif
			} else{
				this->addch(format[i]);
			}
		}
	}
	template<typename... Types>
	void printf(const char* format, std::string_view first_val, Types... next_vals){
		for( int i = 0; format[i] != '\0'; ++i ){
			if( format[i] == '%' ){
				this->addstr(first_val.data());
				return this->printf(format + i + 1, next_vals...);
			} else{
				this->addch(format[i]);
			}
		}
	}

	/**
	 * @important @note - The pairs will be in the form of row*col
	 * @brief - Uses stdscr as the parent screen
	*/
	SubWindow(const std::pair<int, int>& dimensions, const std::pair<int, int>& start_yx) :
		SubWindow(dimensions.first, dimensions.second, start_yx.first, start_yx.second)
	{
		this->enabled = true;
	}
	SubWindow(SubWindow_Ptr& parent_win, const std::pair<int, int>& dimensions, const std::pair<int, int>& start_yx) :
		SubWindow(parent_win, dimensions.first, dimensions.second, start_yx.first, start_yx.second)
	{
		this->enabled = true;
	}

	SubWindow(int height, int width, int y_corner, int x_corner) :
		dimensions{height, width},
		y_corner(y_corner),
		x_corner(x_corner),
		win(subwin(stdscr, height, width, y_corner, x_corner))
	{
		this->updateDimen();
		this->enabled = true;
	}
	SubWindow(SubWindow_Ptr& parent_win, int height, int width, int y_corner, int x_corner) :
		dimensions{height, width},
		y_corner(y_corner),
		x_corner(x_corner),
		parent_win(parent_win),
		win(subwin(parent_win->win.get(), height, width, y_corner + parent_win->dimensions.n_row + 1, x_corner + parent_win->dimensions.n_col + 1))   // the corner params assume to be wrt. to stdscr
	{
		this->updateDimen();
		this->enabled = true;
	}

	~SubWindow(){
		// @note @future - for multi-threading, this next if MUST be atomic (for eg. two owners release the pointers simulateusly but for both this if may give false, that's the problem)
		if( win.use_count() == 1 ){	//  only we ourselves remain as its owner
			// @note @caution - Currently the delwin() calls have been commented, since it's causing double free, since the smart pointer also implicitly calls reset()
			// delwin(win.get());
		}
		if( parent_win && parent_win.use_count() == 1 ){
			// delwin(parent_win->win.get());
		}
	}

private:
	const int y_corner, x_corner;  // data required for enable disable
	struct {
		int n_row;
		int n_col;
	}dimensions;
	struct {
		int n_row{ 0 };
		int n_col{ 0 };
	}last_write_pos;    // last locations to which we wrote anything
	bool boxed{ false };  // @caution - Currently no function is available that resets this to false (though, since it's not required in our case, leaving)
	std::shared_ptr<WINDOW> win;
	SubWindow_Ptr parent_win; // @note - this shared_ptr will hold NOTHING, when parent window is stdscr


};

#endif
