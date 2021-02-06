#pragma once

#include <thread>

#include "id_creator.hpp"
#include "state.hpp"
#include "world_tree.hpp"
#include "adapter.hpp"

typedef int32_t dimen_t;
struct StatePlusPlus{
	const State& state;
	const id_type node_id;
};

class World_Node : public _ID{
	typedef World* World_Ptr;
	typedef World_Node* World_Node_Ptr;

public:
	// std::shared_ptr<Display> dispManager;    // @note - May be needed, else it will have to go this way: this->world->verse->display to get to the display class, and then use the data
	std::shared_ptr<DisplayAdapter> adapter;	// the display controller, shoudl be unique_ptr, but is shared_ptr, since we need a reference in the queue in the Display class, which we do by using a weak_ptr
	World_Tree* tree;

	void update_disp();

	const World_Ptr get_world() const;

	const dimen_t get_world_dimen() const;

	// @note - Simulation -> The constructors themselves will AUTOMATICALLY start the simulation just before constructor finishes, by calling start_simultaion()
	World_Node( World_Tree* tree, World_Node* parent_node, _timePoint t, bool is_continued = false);

	~World_Node();

private:

	bool continued_world{ false };    // stores whether this node just inherited the world pointer from parrent instead of a new world being formed. @note - This is to not delete the same emmory location multiple times, so when the switch is made to smat pointers, do remove it and use the smart pointers that automatically handle the memory for you

	World_Ptr world;
	id_type world_id;   /* @note - this->_id and this->world_id    are not the same for world_node, since world_id can be same, also prefer world_node's _id*/

	std::vector<State> states;    // holds all states from it's formation to vbeing pauseed
	_timePoint paused_time{ 0 };  //time at which it has been paused, will be 0 otherwise (ie. it is currently running)
	// @note - paused_time = 0 & paused_time = statics::BIG_BANG_TIME is different, 0 just means that the world_node is active, and not yet paused

	World_Node_Ptr parent_node; // this being nullptr, means that `this` node is the root itself
	World_Node_Ptr left_node, right_node;   // each node will have at max two childs
	std::mutex node_mutex;  // @note - Try to have this need removed, by managing edge cases where itis need buyt may not be actually needed

	void captureState();

	const StatePlusPlus return_data();

	// this is to be provided to the user to edit how the `current` state should be, this option creates a new worldLine for future
	// this returns `current state`
	const State& return_state();

	// this is to be provided to the user to edit how the `current` state should be, this option creates a new worldLine for future
	// this returns `past state` time t_back back into the past
	const State& return_state(_timePoint t_back);

	// @START_HERE -> LAST TIME I WAS COMPLETETING IMPLEMENTING WORLD_TREE, WITH WORLD_NODE TAKING CHARGE OF MANAGING ALL SIMULATION RELATED TASK OF THE WORLD IT HOLDS A POINTER TO, during doing the latter, i also created the functions return_state and handle_pause, do read the notes given for handle_pause later on
	// actual on screen handling of pause to be done by verse, as well as the display, and then calling return_state(), then asking user to modify it, and then that modified state should be passed to this handle_pause()
	void handle_pause(State that_state);

	void start_logging();
	void stop_WorldSimulation();

	/**This will pause the threads for the time being, and update the time, and states vector, then create a new node with the same World* pointer and the world_id */
	void pauseWorld();

	void stop_display();

	friend class Display;
	friend class World_Tree;
	// @change - Now, World_Tree won't be constructing the root node, rather, the Verse will create it in big_bang(), and then passes it to World_Tree to work further
	// @reverting_change - World_Tree:init() will create the root node now
};
