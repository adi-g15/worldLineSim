#pragma once

#include <thread>

#include "id_creator.hpp"
#include "state.hpp"
#include "world.hpp"
#include "display.hpp"

typedef int32_t dimen_t;
struct StatePlusPlus{
	const State& state;
	const id_type node_id;
	const dimen_t x_dimen;
	const dimen_t y_dimen_t;  // the dimensions of the world
};

class World_Node : public _ID{
	typedef World* World_Ptr;
	typedef World_Node* World_Node_Ptr;

	bool continued_world{ false };    // stores whether this node just inherited the world pointer from parrent instead of a new world being formed. @note - This is to not delete the same emmory location multiple times, so when the switch is made to smat pointers, do remove it and use the smart pointers that automatically handle the memory for you
	World_Ptr world;
	id_type world_id;   /* @note - this->_id and this->world_id    are not the same for world_node, since world_id can be same, also prefer world_node's _id*/

	struct{
		WINDOW* node_window{ nullptr };   // @note - Invalidate this when this node is removed from the view
		int window_dimen_x;
		int window_dimen_y;
	}_window_data;

	std::vector<State> states;    // holds all states from it's formation to vbeing pauseed
	_timePoint paused_time{ 0 };  //time at which it has been paused, will be 0 otherwise (ie. it is currently running)
	// @note - paused_time = 0 & paused_time = statics::BIG_BANG_TIME is different, 0 just means that the world_node is active, and not yet paused

	World_Node_Ptr parent_node; // this being nullptr, means that `this` node is the root itself
	World_Node_Ptr left_node, right_node;   // each node will have at max two childs
	std::mutex node_mutex;  // @note - Try to have this need removed, by managing edge cases where itis need buyt may not be actually needed

	void captureState(){
		this->paused_time = this->world->currentTime;
		State latest_state(this->paused_time);

		for( Entity& entity : world->snakes ){
			latest_state.curr_pos.push_back(entity.getPos());
		}

		this->states.push_back(latest_state);
	}

	const StatePlusPlus return_data(){
		return {
			states.back(),
			this->_id,
			this->world->_WorldDimen,   /*x and y dimensions are same for now*/
			this->world->_WorldDimen
		};
	}

	// this is to be provided to the user to edit how the `current` state should be, this option creates a new worldLine for future
	// this returns `current state`
	const State& return_state(){
		return states[states.size() - 1];   // or simply states.back(), but leave so as to have better understanding of the two overloads
	}

	// this is to be provided to the user to edit how the `current` state should be, this option creates a new worldLine for future
	// this returns `past state` time t_back back into the past
	const State& return_state(_timePoint t_back){   // this only `returns` the current state of 
		if( t_back < states.size() ){ // @caution - Can be concurrently accessing the size and in other thread pushing back state to it
			if( !parent_node ){
				return parent_node->return_state(t_back - states.size());
			} else{
				if( t_back == 0 ){
					throw std::invalid_argument("You are going 0 seconds before the BIG BANG, ie. TO THE BIG BANG, Sorry Can;t handle that now :-) ");
				}
				throw std::invalid_argument("Can't go to even before the BIG BANG. Soooorrry for that :D ");
			}
		}

		return this->states[states.size() - 1 - t_back];    // we are seeing from the reverse, thats why we did -t_back, and we had to -1, since what we receive is 1-based index type something (think logically by drawing a stack of states at t = t` for all t` belonging to {0,..., latest(ie. states.size())})
	}

	// @START_HERE -> LAST TIME I WAS COMPLETETING IMPLEMENTING WORLD_TREE, WITH WORLD_NODE TAKING CHARGE OF MANAGING ALL SIMULATION RELATED TASK OF THE WORLD IT HOLDS A POINTER TO, during doing the latter, i also created the functions return_state and handle_pause, do read the notes given for handle_pause later on
	// actual on screen handling of pause to be done by verse, as well as the display, and then calling return_state(), then asking user to modify it, and then that modified state should be passed to this handle_pause()
	void handle_pause(State that_state){    // the time of pause to be deduced from the time of current world
		// @note - stop_simulation should be called before this handle_pause, likely by the Verse itself, because this function will actually just do the work while the actual world has been `kind of` paused (but at same time it should keep running, or pause and resume when created the new node that continues with this world itself)
		this->paused_time = world->currentTime;

		// @todo - Cr5eating a new world here too, and also a node with this same world
		this->left_node = new World_Node(this->world, this->paused_time, this->dispManager, true); //this will be almost the copy of this node, with the same world pointer, just starting with an empty states vector, and that it's paused_time will also not be there (ie. by default 0, meaning the world_node has an active world currently running)

	}

	void start_simulation(){    // causes the world to start running

		for( auto&& snake : this->world->snakes ){
			this->world->entity_threads.push_back(
				std::thread(&Snake::simulateExistence, snake)
			);
		}

		// @debug - Uncomment next line, just commented it for successful build
		// std::thread(&WorldPlot::start_auto_expansion(), this->world_plot);    // @future - Explore the possibilities of doing the world_plot expansion in this_thread, ie. the world's thread itself (that will likely have modifying the next while loop)

		// this loop `just waits AND ensures the world_plot has food avaialable`, (since the entities are on there own threads)
		while( this->world->_shared_concurrent_data.is_world_running() ){
			if( this->world->world_plot.get_food() == nullptr ) this->world->world_plot.createFood(); //though this if block should NEVER be reached, since ateFood() also creates new Food
			this->captureState();   // can PAUSE then capture, for i am preferring some wrong coords in state, rather than stopping and starting threads again and again

			//pause for a unit time
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(statics::UNIT_TIME * 1000)));
		}

		this->stop_WorldSimulation(); //it will stop all the threads

		++this->world->currentTime;

	}

	void stop_WorldSimulation(){
		this->world->_shared_concurrent_data._world_runnning = false;

		for( auto&& thread : this->world->entity_threads ){
			if( thread.joinable() )
				thread.join();
		}

		// @todo - Do whatever needs to be done, after all entities have been stopped
	}

	/**This will pause the threads for the time being, and update the time, and states vector, then create a new node with the same World* pointer and the world_id */
	void pauseWorld(){
		// @todo
	}

	void remove_window(){   // invalidates the WINDOW* pointer held by this
		this->_window_data.node_window = nullptr;
	}

public:
	Display* dispManager;    // @note - May be needed, else it will have to go this way: this->world->verse->display to get to the display class, and then use the data

	void update_node_disp(){
		if( !this->_window_data.node_window )   return; //  node is not on the screen (ie. node_window is null)

		// mvwaddstr(this->node_window, 1, std::max((x_length - 2 - util::num_digits(this->node_id)) / 2, 1), std::to_string(this->node_id).data());
		mvwaddstr(this->_window_data.node_window, 1, 1, std::to_string(this->_id).data());

		mvwhline(this->_window_data.node_window, 2, 1, ACS_HLINE, this->_window_data.window_dimen_x - 2);
		mvwprintw(this->_window_data.node_window, 3, 1, "Dimen (%d,%d)", this->world->get_curr_bound(),  this->world->get_curr_bound());

		// @note @future - Here for now we are nost asking the display manager to increase the box heught, to accomodate more entitites, this may be solved in future, currently presuming `this->world->snakes(or entitites).size() = 4`
		for( int i = 0; i < this->world->snakes.size(); i++ ){
			auto snake = std::cref(this->world->snakes[i]).get();

			mvwprintw(this->_window_data.node_window, 4 + i, 1, "E%d - (%d, %d), %d", i + 1, snake.getHeadCoord().mX, snake.getHeadCoord().mY, snake.getUniqProp() );
		}
	}

	const World_Ptr get_world() const{
		std::scoped_lock s(node_mutex);

		return this->world;	// @risky
	}
	World_Node(World&) = delete;

	// @note - Be sure you have ALL respective arguments as taken by the World class constructor, since the node itself will need them to construct a new world
	World_Node(World_Ptr old_world, _timePoint t, Display* dispMngr, bool is_continued = false) : /*world(old_world, t), */continued_world(is_continued){
		this->world_id = old_world->_id;

		this->dispManager = dispMngr;
		// @warning @thread -> See all occurences of such cases where we are accessing data members/functions directly using the arrow notation, and ENSURE IT'S THREADSAFE (where they tend to be on different on different threads)
		dispMngr->addNode(this);	// adds this node, as well as initialise this->_display_data

		this->paused_time = world->currentTime;  //initially
	}

	~World_Node(){
		if( !this->continued_world )
			delete this->world;
	}

private:
	World_Node() = delete;
	World_Node(Display* dispMngr) : paused_time(0) {
		this->world = new World();

		this->dispManager = dispMngr;
	// create a new world here
	}

	friend class Display;
	friend class World_Tree;
	// @change - Now, World_Tree won't be constructing the root node, rather, the Verse will create it in big_bang(), and then passes it to World_Tree to work further
	// @reverting_change - World_Tree:init() will create the root node now
};
