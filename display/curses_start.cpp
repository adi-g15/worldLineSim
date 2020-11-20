#include <cstdio>
#include <thread>
#include <chrono>

#include <iostream>
#include <future>

#include "node_window.hpp"

#include "curses.h"
#include "signal.h"

using namespace std::chrono_literals;

WINDOW* top_area = nullptr;
WINDOW* main_area = nullptr;
WINDOW* win_legend = nullptr;

void handleExit(int signal){
	if(top_area != nullptr)
		delwin(top_area);
	if(main_area != nullptr)
		delwin(main_area);
	if(win_legend != nullptr)
		delwin(win_legend);
	endwin();
}

std::future<char> inputFunc(){
	return std::async(
		std::launch::async,
		[](){
			// char ch = getch();
			char ch;

			std::cin >> ch;
			return ch;
			// scanf("%c", &ch);

			// return ch;
		}
	);
}

std::future<char> get_future(){
	return std::async(std::launch::async, []{
				char ch;

				std::cin >> ch;
				return ch;
			});
}

int main(int argc, char const *argv[]){
	// std::cin.tie(0);	// since we will be taking input (actually using stdio functions, so these MAY not be required)
	// std::cout.tie(0);

	#ifdef _GLIBCXX_IOSTREAM
		std::ios_base::sync_with_stdio(false);	// since we will be taking input from other threads
	#endif

	int old_x_dimen, old_y_dimen;	// for responsiveness
	int x_dimen, y_dimen;
	int main_x_dimen, main_y_dimen;

	initscr();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	nonl();	// tells curses not to do NL -> CR/NL on output	(i didn't actually make noticeable changes to the way iut loojed toi jne, but maybe it prioviudes better perfoirnabce)
	noecho();
	cbreak();	// no wait for '\n'; take char one at a time

	signal(SIGTERM, handleExit);

	getmaxyx(stdscr, old_y_dimen, old_x_dimen);

	char QUIT_KEY = 'q';
	int main_y_corner = 3;
	int main_x_corner = 0;
	top_area = subwin( stdscr, 3, 0, 0, 0 );
	main_area = subwin( stdscr, 0, 0.8f*old_x_dimen, main_y_corner, main_x_corner );
	win_legend = subwin( stdscr, 0, 0, 3, 0.8f*old_x_dimen);

	getmaxyx(main_area, main_y_dimen, main_x_dimen);

#if 0
	std::vector<std::vector<bool>> v;
	try{
		v.resize(main_y_dimen, std::vector<bool>(main_x_dimen));
	}catch(...){
		raise(SIGTERM);

		std::cout << " dimensions (y, x) were: " << main_y_dimen << ", " << main_x_dimen << " from 1st one" << std::endl;
		std::cout << " old dimensions (y, x) were: " << old_y_dimen << ", " << old_x_dimen << " from 1st one" << std::endl;
		return 0;
	}

	std::vector<node_window> nodes(8);
	for (int i = 0; i < 8; i++)
	{
		try{
			nodes.push_back( {main_area, main_y_corner, main_x_corner, v, i} );
		}catch(std::exception& e){
			raise(SIGTERM);

			std::cerr << e.what() << " from " << i+1 << "th one" << std::endl;
			return 0;
		}
		wrefresh(main_area);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	box(main_area, ACS_VLINE, ACS_HLINE);
	wrefresh(main_area);
	std::this_thread::sleep_for(std::chrono::seconds(5));
#endif

#if 0	/*We can simply just refresh the child window, to have its borders shown*/
	WINDOW* gr_par = subwin(stdscr, 0, 0, 4, 4);
	WINDOW* par = subwin(gr_par, 0, 0, 8, 10);
	WINDOW* child = subwin(par, 0, 0, 15, 16);

	box(gr_par, '|', '-');
	// wrefresh(gr_par);
	// std::this_thread::sleep_for(2s);
	// refresh();
	std::this_thread::sleep_for(4s);

	box(par, ';', '@');
	// wrefresh(par);
	// std::this_thread::sleep_for(2s);
	// refresh();
	std::this_thread::sleep_for(4s);

	box(child, '^', '&');
	wrefresh(gr_par);
	// std::this_thread::sleep_for(2s);
	// refresh();
	std::this_thread::sleep_for(2s);

	delwin(gr_par);
	delwin(par);
	delwin(child);
	endwin();
#endif

#if 0
	std::this_thread::sleep_for(2s);
	mvwaddstr( node1.window, 3, 1, "hello" );
	wrefresh(node1.window);

	std::this_thread::sleep_for(5s);
	raise(SIGTERM);

	return 0;
#endif

#if 0
	box(stdscr, 'u', 'e');
	wrefresh(stdscr);	// wclear() clears all area inside a window, even if that is also used by other window
	std::this_thread::sleep_for(2s);

	box(win_legend, ACS_VLINE, ACS_UARROW);
	wrefresh(win_legend);
	std::this_thread::sleep_for(4s);
	wclear(stdscr);	// wclear() (and wrefresh() doesn't clear, but affects the same way) clears all area inside a window, even if that is also used by other windo
	wrefresh(stdscr);
	std::this_thread::sleep_for(4s);	


	handleExit(SIGTERM);	return 0;	// DEBUG

#endif

	int counter = 2;	short direction = 1;	// downward (adds +1)
	int legend_counter = 16;
	box(stdscr, ACS_VLINE, ACS_HLINE);

	auto async_task = inputFunc();

#if 0
	async_task.wait();

	waddstr( stdscr, " got -> ");
	try{
		if(async_task.valid()){
			mvaddstr(5,5,"valid");
		}else{
			mvaddstr(5,5,"IN valid");
		}
		auto ch = async_task.get();

		waddch( stdscr, ch);
		refresh();

		std::this_thread::sleep_for(3s);

	}catch(...){
		std::this_thread::sleep_for(3s);
		waddstr( stdscr, " ERROR");
		refresh();

		raise(SIGTERM);
	}
#endif

	mvwaddstr(win_legend, 1, 1, "Legend");
	mvwaddstr(win_legend, 2, 1, "*All worlds continue");
	mvwaddstr(win_legend, 3, 1, "on diff. threads,w/o");
	mvwaddstr(win_legend, 4, 1, "blocking the display,");
	mvwaddstr(win_legend, 5, 1, " or the verse");

	mvwaddstr(win_legend, 7, 1, "- Type the id to chose a");
	mvwaddstr(win_legend, 8, 1, " particular world");
	mvwaddstr(win_legend, 9, 1, "- Commands");
	mvwaddstr(win_legend, 10, 1, "-   N - Namaste World(New)");
	mvwaddstr(win_legend, 11, 1, "-   P - Pause");
	mvwaddstr(win_legend, 12, 1, "-   R - Resume");
	mvwaddstr(win_legend, 13, 1, "-   T - Time Travel !!");
	mvwaddstr(win_legend, 14, 1, "-   L - Logs (of World)");
	mvwaddstr(win_legend, 15, 1, "-   V - Logs (of Verse)");

	mvwaddstr(win_legend, getmaxy(win_legend) - 3, 1, "If you find a problem...");
	mvwaddstr(win_legend, getmaxy(win_legend) - 2, 1, "Please solve it yourselves");
	mvwaddstr(win_legend, getmaxy(win_legend) - 1, 1, ":copy: AdityaG15 :D");

	while ( true ){

		getmaxyx(stdscr, y_dimen, x_dimen);
		getmaxyx(main_area, main_y_dimen, main_x_dimen);
		if( x_dimen != old_x_dimen || y_dimen != old_y_dimen )
		{
			if( x_dimen > old_x_dimen || y_dimen > old_y_dimen )
			{
				wclear(stdscr); // to clean out the previous borders

			}

			old_x_dimen = x_dimen;
			old_y_dimen = y_dimen;
			// box(stdscr, ACS_VLINE, ACS_HLINE);	// now since we have divided into sub windows, no need for this now, for boxing completely
		}

		wclear(main_area); // to clean out the previous borders
		box(top_area, ACS_VLINE, ACS_HLINE);
		box(main_area, ACS_VLINE, ACS_HLINE);
		box(win_legend, ACS_VLINE, ACS_HLINE);

		mvwaddstr( top_area, 1, getmaxx(top_area)/2 - 10, "WorldLine Simulator" );

		// wclear(win_legend);	// not required, after using subwin
		// box(win_legend, ACS_VLINE, ACS_UARROW);
		// wrefresh(win_legend);	// not required, after using subwin

		wmove(main_area, counter,1);	// y, x
		waddstr(main_area, "x_dimen -> ");
		waddstr(main_area, std::to_string(main_x_dimen).data());
		waddstr(main_area, "	y_dimen -> ");
		waddstr(main_area, std::to_string(main_y_dimen).data());

		wrefresh(main_area);
		wrefresh(win_legend);

		if(counter >= main_y_dimen-2 || counter == 1)	direction = (direction == 1) ? -1 : 1;	// convert from 1 to -1, and vice versa
		counter += direction;

		wrefresh(stdscr);

			// time to take input
		if( async_task.wait_for(200ms) == std::future_status::ready ){
			++legend_counter;
			if( legend_counter >= (getmaxy(win_legend)-1) )	legend_counter = getmaxy(win_legend)/2;

			mvwaddstr(win_legend, legend_counter, 1, "You typed -> ");
			try{
				auto c = async_task.get();
				waddch(win_legend, c);

				if(c == QUIT_KEY){
					raise(SIGTERM);

					return 0;
				}
				wrefresh(win_legend);
			}catch(std::future_error & e){
				raise(SIGTERM);

				std::cout << std::boolalpha << async_task.valid() << std::endl;
				std::cerr << e.code()<< " " << e.what() << std::endl;
				return 0;
			}

			wrefresh(win_legend);
			async_task = inputFunc();	// restart that task
		}

		std::this_thread::sleep_for(200ms);
	}

	raise(SIGTERM);	// signal SIGTERM to itself
	return 0;
}
