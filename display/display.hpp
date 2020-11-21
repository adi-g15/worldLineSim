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
#include "curses_subwin.hpp"

class node_adapter;	// forward-declaration

	// @note - The queue member functions return World_Node* not the adapters (The adapters are meant to be modified by this class itself)
struct _8_node_queue{   // @note - It has been separately defined as a wrapper class to std::queue, specially to be used by class Display, and to avoid all these if statements checking always in different functions that will be operating with the queue
	std::list<World_Node*> data;   // @note - We may need to use list, since we also want random access, say to the 5th node currently on display on the screen
	// std::list<std::unique_ptr<node_adapter>> adapters; 
	std::list< node_adapter > adapters; 

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
			this->data.pop_front();
			this->adapters.pop_front();
		}

		auto size(){    // sure to be <= 8
			return data.size();
		}
	// END - Typical Queue Operations

		_8_node_queue(){}

};

/* This class will just `hold` everything, with all handling of `its members` being controlled by the parent verse object */
class Display : public single_term, public std::enable_shared_from_this<Display>{
	typedef std::shared_ptr<SubWindow> SubWindow_Ptr;


	Verse* parent_verse;

	static constexpr char QUIT_KEY{'q'};

	int adapters_height{10};
	int adapters_width{17};	// these should be same for all, as of the current situation
		// @future - Try to make these adapt to, for eg. more snakes, so ONLY the next code for determining the position should need change, after the y_length, and x_length aren't same for all node_adapters anymore

	_8_node_queue queue; // pointers to world_nodes

	SubWindow_Ptr top_area, main_area, legend_area;

	std::vector<std::vector<bool>> occupy_table;	/* the occupy table keeps record of what regions are occupied (also called, occupancy_table) */

	public:
	bool paused{ true };

	node_adapter newNodeAdapter(World_Node* node);
	void runInitTasks() override;
	void showExit();
	void helpScreen();
	void optionScreen();	// shows options -> 1. View verse state; 2. View help
	void runPreEndTasks();
	void resumeRendering();
	void pauseRendering();

	Display() = delete;
	Display(Verse*);
	~Display();

	friend class node_adapter;
	friend class Verse;
};
