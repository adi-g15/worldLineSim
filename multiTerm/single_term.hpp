#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <future>
#include <thread>

#include "signal.h"
#include "curses.h"

static void sig_handler(int SIGNAL){   // @warning -> It doesn't close the program itself, do that yourself (since way of doing so can be different according to preferences, for eg. i prefer using exceptions many a times)
	static bool sig_handled = false;
	if( sig_handled )	return;	// if signal already handles, then silently return
		// @note - This handler maybe called twice due to the reason, a function calls single_term::reset_curses, and also raise(SIGTERM) is called (see for example, display::runInitTasks, where entering 'q' first calls reset_curses and then raise(SIGTERM) too), these cases may just have the raise() call removed, but it's there to make sense

	endwin();
	sig_handled = true;
}


/*
	Meant to be inherited from
*/
class single_term{  //initially will have 3 windows, a square one at bottom left, rectangular at bottom right, and a rectangular intoduction at top
	bool curses_initialized{ false };
	std::vector< WINDOW* > _Windows;	// not in use currently, in Display class
	protected:
	std::string title;
	std::string description;    // @note - Not now (not enough space)

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
		wrefresh(stdscr);
	}

	void refreshWindow(int win_index){
		wrefresh(this->_Windows[win_index]);
	}

	void checkResize(){
		// @future @me - Currently it will be managed inside the overloaded runInitTasks() member function, by the child class, though this function may contain a template like stuff for such a support, though it should not be used here
	}

	static std::future<char> get_async_input(){
		return std::async(
			std::launch::async,
			[](){
				char ch;
				std::cin >> ch;

				return ch;
			}
		);
	}

	void reset_curses(){
		if( !this->curses_initialized )	return;

		for( WINDOW* win : _Windows ){
			delwin(win);
		}

		raise(SIGTERM);	// which then calls endwin()
	}

	// @note - by default, curses_init will show an empty box
	void curses_init(){
		// @warning - Before calling raise(), be sure to call 
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
		keypad(stdscr, TRUE);	// not being used in worldLine simulator
		nonl(); // tells cursor not to do NL -> CR/NL on output
		noecho();
		cbreak();   // no wait for '\n', one char at a time

		this->curses_initialized = true;
		getmaxyx(stdscr, this->_terminal_y, this->_terminal_x);

		this->clearAll();
	}
	virtual void runInitTasks() = 0;

	//currently number of windows is fixed, since this is not the primary goal, this will be as such for some time maybe
	single_term() = default;
	single_term(std::string_view title,std::string_view desc) : title(title), description(desc){}
	~single_term(){
		if( !curses_initialized ){
			return;
		}

		for( WINDOW* win : _Windows ){
			delwin(win);
		}

		endwin();
	}
};
