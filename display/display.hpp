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
			this->data.erase(this->data.begin());
		}

		auto size(){    // sure to be <= 8
			return data.size();
		}
	// END - Typical Queue Operations

		auto add_adapter(node_adapter& adapter){
			this->push(adapter.node);
			this->adapters.push_back(adapter);
		}

		_8_node_queue(){}

};

/* This class will just `hold` everything, with all handling of `its members` being controlled by the parent verse object */
class Display : public single_term{
	typedef std::shared_ptr<SubWindow> SubWindow_Ptr;


	Verse* parent_verse;

	static constexpr char QUIT_KEY{'q'};

	_8_node_queue queue; // pointers to world_nodes

	SubWindow_Ptr top_area, main_area, legend_area;

	std::vector<std::vector<bool>> occupy_table;

	public:
	node_adapter newNodeAdapter(World_Node* node){
		// Initialisizes a node_adapter, and the SubWindow for that, then return BY VALUE
		// node->dispManager = this;	// @caution - Assign this in world tree itself, where the node is created, since here, we are getting invalid use of incomplete types due to the order in which the headers have been included, can't do much, else just have two hpp files one with definitons, but i want it to be simpler

		// node_adapter n(this);	// give the current displayManager
		// this->queue.add_adapter(n);	// @come_back - Complete it later
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
