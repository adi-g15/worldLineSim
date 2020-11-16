#pragma once

#include <memory>
#include <queue>
#include <chrono>
#include <thread>
#include <future>

#include "verse.hpp"
#include "forward_decl.hpp"
#include "multiTerm/single_term.hpp"

struct _8_node_queue{   // @note - It has been separately defined as a wrapper class to std::queue, specially to be used by class Display, and to avoid all these if statements checking always in different functions that will be operating with the queue
	std::queue<World_Node*> data;   // @note - We may need to use list, since we also want random access, say to the 5th node currently on display on the screen

	public:
		auto front(){
			return data.front();
		}

		auto back(){
			return data.back();
		}

		auto push(World_Node* node){
			if(data.size() >= 8)      data.pop();

			return data.push(node);
		}

		auto pop(){
			return data.pop();
		}

		auto size(){    // sure to be <= 8
			return data.size();
		}
};

/* This class will just `hold` everything, with all handling of `its members` being controlled by the parent verse object */
class Display : public single_term{
	Verse* parent_verse;

	static constexpr char QUIT_KEY{'q'};

	_8_node_queue nodes; // pointers to world_nodes

	WINDOW *top_area, *main_area, *legend_area;
	int main_y_corner, main_x_corner;
	int main_dimen_y, main_dimen_x;


	public:
	void addNode(World_Node* node){
		// node->dispManager = this;	// @caution - Assign this in world tree itself, where the node is created, since here, we are getting invalid use of incomplete types due to the order in which the headers have been included, can't do much, else just have two hpp files one with definitons, but i want it to be simpler

		this->nodes.push(node);
	}
	void runInitTasks() override;
	void showExit();
	void runPreEndTasks();

	Display() = delete;
	Display(Verse*);
	~Display();

	friend class Verse;
};

void Display::runInitTasks(){
	using namespace std::chrono_literals;

	// if( std::this_thread::get_id() == this->parent_verse->thread_id ){
	//     ERROR: DISPLAY SHOULD BE ON A DIFFERENT THREAD, THAN THE VERSE
	// }

	this->curses_init();

	this->clearAll();

	main_y_corner = 3;
	main_x_corner = 0;
	this->top_area = subwin( stdscr, 3, 0, 0, 0 );
	this->main_area = subwin( stdscr, 0, 0.8f * _terminal_x, main_y_corner, main_x_corner );
	this->legend_area = subwin( stdscr, 0, 0, 3, 0.8f*_terminal_x );

	this->_Windows.reserve(3);
	_Windows[0] = this->top_area;
	_Windows[1] = this->main_area;
	_Windows[2] = this->legend_area;

	auto async_input = this->get_async_input();
	getmaxyx(main_area, main_dimen_y, main_dimen_x);

	mvwaddstr(legend_area, 1, 1, "Legend");
	mvwaddstr(legend_area, 2, 1, "*All worlds continue");
	mvwaddstr(legend_area, 3, 1, "on diff. threads,w/o");
	mvwaddstr(legend_area, 4, 1, "blocking the display,");
	mvwaddstr(legend_area, 5, 1, " or the verse");

	mvwaddstr(legend_area, 7, 1, "- Type the id to chose a");
	mvwaddstr(legend_area, 8, 1, " particular world");
	mvwaddstr(legend_area, 9, 1, "- Commands");
	mvwaddstr(legend_area, 10, 1, "-   N - Namaste World(New)");
	mvwaddstr(legend_area, 11, 1, "-   P - Pause");
	mvwaddstr(legend_area, 12, 1, "-   R - Resume");
	mvwaddstr(legend_area, 13, 1, "-   T - Time Travel !!");
	mvwaddstr(legend_area, 14, 1, "-   L - Logs (of World)");
	mvwaddstr(legend_area, 15, 1, "-   V - Logs (of Verse)");

	mvwaddstr(legend_area, getmaxy(legend_area) - 3, 1, "If you find a problem...");
	mvwaddstr(legend_area, getmaxy(legend_area) - 2, 1, "Please solve it yourselves");
	mvwaddstr(legend_area, getmaxy(legend_area) - 1, 1, ":copy: AdityaG15 :D");

	int cur_dimen_y, cur_dimen_x;
	while( true ){
		getmaxyx(stdscr, this->_terminal_y, this->_terminal_x);
		getmaxyx(main_area, main_dimen_y, main_dimen_x);

		getmaxyx(stdscr, cur_dimen_y, cur_dimen_x);
		if( cur_dimen_y != _terminal_y || cur_dimen_x != _terminal_x ){
			wclear(stdscr); // to clean out the previous borders
		}

		box(top_area, ACS_VLINE, ACS_HLINE);
		box(main_area, ACS_VLINE, ACS_HLINE);
		box(legend_area, ACS_VLINE, ACS_HLINE);

		mvwaddstr( top_area, 1, (getmaxx(top_area) - this->title.size())/2, this->title.data() );

		wrefresh(stdscr);

			// time to take input
		if( async_input.wait_for(100ms) == std::future_status::ready ){
			mvwaddstr(legend_area, static_cast<int>(0.75f*getmaxy(legend_area)), 1, "You entered -> ");
			try{
				auto c = async_input.get();
				waddch(legend_area, c);

				if(c == Display::QUIT_KEY){
					this->reset_curses();	// first free up memory
					raise(SIGTERM);

					// @note - Or maybe use inter thread communication here, using condition_variable, so to wait till verse tells all done, then this will return, and the destructor again sends signal to verse after all things done in destructor, then verse can be sure that all destructed
					// @note - UNCOMMENT next line, later
					// return this->parent_verse->kaal_day("Display");    // also passing the source so that it doesn't actually try to end this
					return;
				}
				wrefresh(legend_area);
			}catch(std::future_error & e){
				raise(SIGTERM);

				// verse::LOGGER << std::boolalpha << async_input.valid() << std::endl;
				// verse::ERR_LOGGER << e.code()<< " " << e.what() << std::endl;
				this->parent_verse->kaal_day("Display");
				return;
			}

			wrefresh(legend_area);
			async_input = this->get_async_input();	// restart that task
		}

		std::this_thread::sleep_for(200ms);
	}

}

void Display::showExit(){
	clear();
	clearAll();

	mvwaddstr(stdscr, this->_terminal_y/2, (this->_terminal_x - 15)/2, "About to Exit !");
}

Display::Display(Verse* parent) :
	single_term("WorldLine Simulator v0.271", "Created by Aditya Gupta and Steins; Gate"),
	parent_verse(parent){}

Display::~Display(){

}
