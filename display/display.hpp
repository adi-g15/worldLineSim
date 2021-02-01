#pragma once

#include <memory>
#include <queue>
#include <list>
#include <chrono>
#include <thread>
#include <future>
#include <mutex>	// for std::call_once, and std::once_flag

#include "forward_decl.hpp"
#include "multiTerm/single_term.hpp"
#include "display/curses_subwin.hpp"
#include "display/node_adapter.hpp"

	// @note - The queue member functions return World_Node* not the adapters (The adapters are meant to be modified by this class itself)
struct _8_node_queue{   // @note - It has been separately defined as a wrapper class to std::queue, specially to be used by class Display, and to avoid all these if statements checking always in different functions that will be operating with the queue
	std::list<World_Node*> data;   // @note - We may need to use list, since we also want random access, say to the 5th node currently on display on the screen
	// std::list<std::unique_ptr<node_adapter>> adapters; 
	std::list< std::weak_ptr<node_adapter> > adapters; 

	public:
	// START - Typical Queue Operations
		auto front() noexcept {
			return data.front();
		}

		auto back() noexcept {
			return data.back();
		}

		auto push(World_Node* node, std::shared_ptr<node_adapter>& adapter) {
			if (data.size() >= 8)      this->pop();

			this->data.push_back(node);
			this->adapters.push_back(adapter);
		}

		void pop(){
			this->data.pop_front();
			this->adapters.pop_front();
		}

		auto size() noexcept {    // sure to be <= 8
			return data.size();
		}
	// END - Typical Queue Operations

		_8_node_queue(){}

};

/* This class will just `hold` everything, with all handling of `its members` being controlled by the parent verse object */
class Display : protected single_term, public std::enable_shared_from_this<Display>{
	typedef std::shared_ptr<SubWindow> SubWindow_Ptr;
	typedef std::shared_ptr<Display> Display_Ptr;

public:
	bool paused{ true };
//	std::once_flag disp_once_flag;	// @deprecated - Was in use when std::call_once was being used to call updateDisplay()

	std::shared_ptr<node_adapter> newNodeAdapter(World_Node* node);
	void runInitTasks() override;
	void printScreen();
	void updateScreen();

//	std::condition_variable event_convar;	// not needed, work done by mutex
	std::mutex event_mutex;	// used to lock mutex so that ONLY ONE thread accesses it during each time interval the display is active
	void startEventLoop();

	/**
	* @note - Display::render() shouldn't be used anymore
	* @why - Since curses is not gauranteed to be thread safe
	* 
	* What it was doing is, in a while loop, every 200ms it will update the screen
	* Consider the scenario, when a node_adapter WRITES to it's own subwindow, (WHICH IS PART OF the main_area), and simultaneously, Display::render writes something at same position through main_area, this is the problem, this shouldn't be a problem but i am not sure if modifying a window and its subwindow is safe or not
	* 
	* @implications -> Since we wont use it, we will need 3 functions -> printScreen(), updateScreen() AND a clickHandler()
	* Also, i first decided to use std::call_once for calls to Display::updateScreen()
	* But, now i have found std::timed_mutex, it seems more appropriate, ie. the function will be called once for the interval, and also it will be waiting for that much interval, GREAT
	*/
	void render();
	void showExit();
	void helpScreen();	// @bug - Doesn't work as of 21st Nov 20
	void optionScreen();	// shows options -> 1. View verse state; 2. View help
	void runPreEndTasks();
	void resumeRendering();
	void pauseRendering();

	Display(Display&) = delete;
	Display(Verse*);
	~Display();

private:
	Verse* parent_verse;

	static constexpr char QUIT_KEY{ 'q' };
	static constexpr char HELP_KEY{ 'h' };

	int adapters_height{ 10 };
	int adapters_width{ 17 };	// these should be same for all, as of the current situation
		// @future - Try to make these adapt to, for eg. more snakes, so ONLY the next code for determining the position should need change, after the y_length, and x_length aren't same for all node_adapters anymore

	_8_node_queue queue; // pointers to world_nodes

	SubWindow_Ptr top_area, main_area, legend_area;

	std::vector<std::vector<bool>> occupy_table;	/* the occupy table keeps record of what regions are occupied (also called, occupancy_table) */
	std::timed_mutex dispMutex;

	struct
	{
	private:
		std::function<void(char, Display_Ptr)> kbhit = [](char key, Display_Ptr source) {
			if (source == nullptr) {
				throw std::invalid_argument("Source to kbhit handler is null");
			}

			if (key == Display::QUIT_KEY) {
				source->reset_curses();	// free up memory
				source->pauseRendering();
			}
		};

		friend void Display::startEventLoop();	// @note @important -> MUST be on a different thread than main thread
	} handlers;

	friend class node_adapter;
	friend class Verse;
};
