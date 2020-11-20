#pragma once

#include <memory>
#include <queue>
#include <list>
#include <chrono>
#include <thread>
#include <future>

#include "verse.hpp"
#include "forward_decl.hpp"
#include "multiTerm/single_term.hpp"
#include "node_adapter.hpp"
#include "curses_subwin.hpp"

	// @note - The queue member functions return World_Node* not the adapters (The adapters are meant to be modified by this class itself)
struct _8_node_queue{   // @note - It has been separately defined as a wrapper class to std::queue, specially to be used by class Display, and to avoid all these if statements checking always in different functions that will be operating with the queue
	std::list<World_Node*> data;   // @note - We may need to use list, since we also want random access, say to the 5th node currently on display on the screen
	std::list<std::unique_ptr<node_adapter>> adapters; 

	public:
	// START - Typical Queue Operations
		auto front(){
			return data.front();
		}

		auto back(){
			return data.back();
		}

		auto push(World_Node* node){
			if(data.size() >= 8)      this->pop();

			return this->data.push_back(node);
		}

		void pop(){
			this->data.erase(this->data.begin());
		}

		auto size(){    // sure to be <= 8
			return data.size();
		}
	// END - Typical Queue Operations

		auto add_adapter(World_Node* node){
			this->push(node);

			this->adapters.push_back(new node_adapter());
		}

		_8_node_queue(){}

};

/* This class will just `hold` everything, with all handling of `its members` being controlled by the parent verse object */
class Display : public single_term{
	typedef std::shared_ptr<SubWindow> SubWindow_Ptr;


	Verse* parent_verse;

	static constexpr char QUIT_KEY{'q'};

	_8_node_queue nodes; // pointers to world_nodes

	SubWindow_Ptr top_area, main_area, legend_area;

	std::vector<std::vector<bool>> occupy_table;

	public:
	node_adapter newNodeAdapter(World_Node* node){
		// Initialisizes a node_adapter, and the SubWindow for that, then return BY VALUE
		// node->dispManager = this;	// @caution - Assign this in world tree itself, where the node is created, since here, we are getting invalid use of incomplete types due to the order in which the headers have been included, can't do much, else just have two hpp files one with definitons, but i want it to be simpler

		this->nodes.push(node);
	}
	void runInitTasks() override;
	void showExit();
	void helpScreen();
	void runPreEndTasks();
	void pauseRendering();

	Display() = delete;
	Display(Verse*);
	~Display();

	friend class Verse;
};

void Display::pauseRendering(){
	
}

void Display::helpScreen(){
	using namespace std::chrono_literals;

	this->curses_init();	// @note - single_term itself manages if curses is already initialised

	this->main_area->disable();
	this->legend_area->disable();

	SubWindow helpArea{ 0, 0, 3, 0 };	// don't make it a raw pointer, since for it's destructor to be called, it's scope should end, which won't happen if we use new SubWindow, with raw pointer

	

}

void Display::runInitTasks(){
	using namespace std::chrono_literals;

	// if( std::this_thread::get_id() == this->parent_verse->thread_id ){
	//     ERROR: DISPLAY SHOULD BE ON A DIFFERENT THREAD, THAN THE VERSE
	// }

	this->curses_init();

	this->clearAll();

	if( !top_area ) this->top_area.reset( new SubWindow( 3, 0, 0, 0 ) );
	if( !main_area ) this->main_area.reset( new SubWindow( 0, 0.8f * _terminal_x, 3, 0 ) );
	if( !legend_area ) this->legend_area.reset( new SubWindow( 0, 0, 3, 0.8f*_terminal_x ) );

	if( top_area->enabled ) this->top_area->enable();
	if( main_area->enabled ) this->top_area->enable();
	if( legend_area->enabled ) this->top_area->enable();

	auto async_input = this->get_async_input();

	legend_area->addstr(1, 1, "Legend");

	legend_area->nladdstr("*All worlds continue on diff. threads,w/o blocking the display, or the verse");

	legend_area->mvnextLine();

	legend_area->nladdstr("- Type the id to chose a particular world");

	legend_area->nladdstr("- Commands");
	legend_area->nladdstr("-   N - Namaste World(New)");
	legend_area->nladdstr("-   P - Pause");
	legend_area->nladdstr("-   R - Resume");
	legend_area->nladdstr("-   T - Time Travel !!");
	legend_area->nladdstr("-   L - Logs (of World)");
	legend_area->nladdstr("-   V - Logs (of Verse)");

	legend_area->addstr(-3, 1, "If you find a problem...");
	legend_area->nladdstr("Please solve it yourselves");
	legend_area->nladdstr(":copy: AdityaG15 :D");

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
			legend_area->addstr(static_cast<int>(0.75f*getmaxy(legend_area)), 1, "You entered -> ");
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
