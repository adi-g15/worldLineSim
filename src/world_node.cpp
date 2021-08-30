#include "world.hpp"
#include "world_node.hpp"
#include "display/display.hpp"
#include "adapter.hpp"

void World_Node::capture_state(){
    this->states.emplace_back(this);  // for some reason the 1st element is always being tried to be removed
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
    // @todo - Cr5eating a new world here too, and also a node with this same world
    //this->diverged_child = new World_Node(this, this->current_time, true); //this will be almost the copy of this node, with the same world pointer, just starting with an empty states vector, and that it's paused_time will also not be there (ie. by default 0, meaning the world_node has an active world currently running)

}

    // should be on a different thread than main thread
void World_Node::start_state_management(){
    while( this->world.is_world_running() ){
        ++this->current_time;
        this->capture_state();   // can PAUSE then capture, for i am preferring some wrong coords in state, rather than stopping and starting threads again and again
        this->adapter->update();

        std::this_thread::sleep_for(statics::UNIT_TIME);
    }
}

void World_Node::end_simulation() {
    this->world.end_simulation();
}

void World_Node::resume_simulation() {
    this->world.resume_simulation();
}

void World_Node::pause_simulation(){
    this->world.pause_simulation();
}

/**This will pause the threads for the time being, and update the time, and states vector, then create a new node with the same World* pointer and the world_id */
const World& World_Node::get_world() const{
    return this->world;
}

void World_Node::diverge_without_change()
{
    this->diverged_child.push_back( new World_Node(this, this->return_state()) );
}

void World_Node::diverge_with_state(const State& custom_state)
{
    this->diverged_child.push_back(new World_Node(this, custom_state));
}

const _timePoint World_Node::getCurrentTime() const
{
    return this->current_time;
}

const dimen_t World_Node::get_world_order() const{
    return this->world.get_world_dimen();
}

const coord& World_Node::get_exact_dimen() const {
    return this->world.get_dimensions();
}

// ie. the root node... create a new world
World_Node::World_Node(World_Tree* tree):
    tree(tree),
    start_time(statics::BIG_BANG_TIME),
    current_time(statics::BIG_BANG_TIME),
    world()
{
    this->adapter = tree->access_disp_manager()->add_node_adapter(this);
    std::thread(&World_Node::start_state_management, this).detach();
}

    // this creates a world 'WITHOUT' any changes in state
World_Node::World_Node( World_Node* parent_node, _timePoint t) : 
    tree(parent_node->tree),
    start_time(t),
    current_time(t),
    parent_node(parent_node),
    world(parent_node->return_state(t))
{
    this->adapter = tree->access_disp_manager()->add_node_adapter(this);
    std::thread(&World_Node::start_state_management, this).detach();
}

World_Node::World_Node(World_Node* parent_node, const State& prev_state):
    tree(parent_node->tree),
    start_time(prev_state.state_time),
    current_time(prev_state.state_time),
    parent_node(parent_node),
    world(prev_state)
{
    this->adapter = tree->access_disp_manager()->add_node_adapter(this);
    std::thread(&World_Node::start_state_management, this).detach();
}

World_Node::~World_Node(){
    auto tmp = this;
    std::stack<World_Node_Ptr> all_nodes;   // i need to delete the leaf nodes, first but then recursively go up the tree too

    all_nodes.push(this);
    // delete leaf nodes first
    while (!all_nodes.empty())
    {
        auto& curr_node = all_nodes.top();

        for (auto& child : curr_node->diverged_child)
        {
            all_nodes.push(child);
        }

        while (all_nodes.top()->diverged_child.empty())
        {
            all_nodes.top()->parent_node->diverged_child.remove(
                all_nodes.top()
            );
            delete all_nodes.top();

            all_nodes.pop();
        }
    }
}

const id_type World_Node::world_id() const noexcept
{
    return this->world._id;
}
