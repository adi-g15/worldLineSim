#include "world.hpp"
#include "world_node.hpp"
#include "display/display.hpp"
#include "display/node_adapter.hpp"

void World_Node::captureState(){
    this->paused_time = this->world->currentTime;
    State latest_state(this->paused_time);

    for( Entity& entity : world->snakes ){
        if (entity.getPrimaryPos()) {}
        else break;

        latest_state.curr_pos.push_back(entity.getPrimaryPos()->get().point_coord);
    }

    this->states.push_back(latest_state);
}

const StatePlusPlus World_Node::return_data(){
    return {
        states.back(),
        this->_id
    };
}

// this is to be provided to the user to edit how the `current` state should be, this option creates a new worldLine for future
// this returns `current state`
const State& World_Node::return_state(){
    return states[states.size() - 1];   // or simply states.back(), but leave so as to have better understanding of the two overloads
}

// this is to be provided to the user to edit how the `current` state should be, this option creates a new worldLine for future
// this returns `past state` time t_back back into the past
const State& World_Node::return_state(_timePoint t_back){   // this only `returns` the current state of 
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
void World_Node::handle_pause(State that_state){    // the time of pause to be deduced from the time of current world
    // @note - stop_simulation should be called before this handle_pause, likely by the Verse itself, because this function will actually just do the work while the actual world has been `kind of` paused (but at same time it should keep running, or pause and resume when created the new node that continues with this world itself)
    this->paused_time = world->currentTime;

    // @todo - Cr5eating a new world here too, and also a node with this same world
    this->left_node = new World_Node(this->tree, this, this->paused_time, true); //this will be almost the copy of this node, with the same world pointer, just starting with an empty states vector, and that it's paused_time will also not be there (ie. by default 0, meaning the world_node has an active world currently running)

}

void World_Node::start_simulation(){    // causes the world to start running

    return; // world_node won't actually simulate the world, let the world do it (this is in opposite of what i earlier though though, think MORE)

    for( auto&& snake : this->world->snakes ){
        this->world->entity_threads.push_back(
            std::thread(&Snake::simulateExistence, snake)
        );
    }

    // @debug - Uncomment next line, just commented it for successful build
    // std::thread(&WorldPlot::auto_expansion(), this->world_plot);    // @future - Explore the possibilities of doing the world_plot expansion in this_thread, ie. the world's thread itself (that will likely have modifying the next while loop)

    // this loop `just waits AND ensures the world_plot has food avaialable`, (since the entities are on there own threads)
    while( this->world->_shared_concurrent_data.is_world_running() ){
        //if( this->world->world_plot.get_food() == nullptr ) this->world->world_plot.createFood(); //though this if block should NEVER be reached, since ateFood() also creates new Food
        this->captureState();   // can PAUSE then capture, for i am preferring some wrong coords in state, rather than stopping and starting threads again and again

        //pause for a unit time
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(statics::UNIT_TIME * 1000)));
    }

    this->stop_WorldSimulation(); //it will stop all the threads

    ++this->world->currentTime;

}

void World_Node::stop_WorldSimulation(){
    this->world->_shared_concurrent_data.reset_world_running();

    for( auto&& thread : this->world->entity_threads ){
        if( thread.joinable() )
            thread.join();
    }

    // @todo - Do whatever needs to be done, after all entities have been stopped
}

/**This will pause the threads for the time being, and update the time, and states vector, then create a new node with the same World* pointer and the world_id */
void World_Node::pauseWorld(){
    // @todo
}

void World_Node::stop_display(){   // invalidates the WINDOW* pointer held by this
    this->adapter.reset();
}

void World_Node::update_disp(){
    if( ! this->adapter ){
        this->adapter->update();
    }
}

const World_Ptr World_Node::get_world() const{
    return this->world;
}

const dimen_t World_Node::get_world_dimen() const{
    return this->world->get_curr_bound();
}

// @note - Be sure you have ALL respective arguments as taken by the World class constructor, since the node itself will need them to construct a new world
World_Node::World_Node( World_Tree* tree ) : continued_world(false), adapter(tree->access_disp_manager()->newNodeAdapter(this)){

}

World_Node::World_Node() : continued_world(false), adapter(nullptr)
{
    this->world = new World();
}

World_Node::World_Node( World_Tree* tree, World_Node* parent_node, _timePoint t, bool is_continued) : /*world(old_world, t), */continued_world(is_continued), adapter(tree->access_disp_manager()->newNodeAdapter(this)){
    if( ! is_continued ){
        this->world = new World( parent_node->world, t );	// starting from parent_node->world
    }else
    {
        this->world = parent_node->world;
    }

    this->tree = tree;
    this->world_id = this->world->_id;

    // this->dispManager = dispMngr;
    // @warning @thread -> See all occurences of such cases where we are accessing data members/functions directly using the arrow notation, and ENSURE IT'S THREADSAFE (where they tend to be on different on different threads)
    // dispMngr->addNode(this);	// adds this node, as well as initialise this->_display_data

    this->paused_time = world->currentTime;  //initially
}

World_Node::~World_Node(){
    if( !this->continued_world )
        delete this->world;
}
