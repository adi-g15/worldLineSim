#pragma once

#include "util/tree.hpp"
#include <vector>
#include <queue>
#include <thread>

/*
NOTE - This time, i will first complete World_Tree class from start, and then what seems to be common to other trees, i will slowly shift it to the tree class, and then all those properties are to inhgherited in the Wrd_tree class from Tree class
*/

struct World_Node{
    typedef World* World_Ptr;
    typedef World_Node* World_Node_Ptr;

    World_Ptr world;
    id_type world_id;
    std::vector<State> states;    // holds all states from it's formation to vbeing pauseed
    _timePoint pause_time{ 0 };  //time at which it has been paused, will be 0 otherwise (ie. it is currently running)
    // @note - pause_time = 0 & pause_time = statics::BIG_BANG_TIME is different, 0 just means that the world_node is active, and not yet paused

    World_Node_Ptr parent_node; // this being nullptr, means that `this` node is the root itself
    World_Node_Ptr left_node, right_node;   // each node will have at max two childs
    std::mutex node_mutex;  // @note - Try to have this need removed, by managing edge cases where itis need buyt may not be actually needed

    void captureState(){
        this->pause_time = this->world->currentTime;
        State latest_state(this->pause_time);

        for( Entity& entity : world->snakes ){
            latest_state.curr_pos.push_back(entity.getPos());
        }

        this->states.push_back(latest_state);
    }

    // this is to be provided to the user to edit how the `current` state should be, this option creates a new worldLine for future
    // this returns `current state`
    State return_state(){
        return states[states.size() - 1];   // or simply states.back(), but leave so as to have better understanding of the two overloads
    }

    // this is to be provided to the user to edit how the `current` state should be, this option creates a new worldLine for future
    // this returns `past state` time t_back back into the past
    State return_state(_timePoint t_back){   // this only `returns` the current state of 
        if(t_back < states.size()){ // @caution - Can be concurrently accessing the size and in other thread pushing back state to it
            if( !parent_node ){
                return parent_node->return_state( t_back - states.size() );
            }else{
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
        this->pause_time = world->currentTime;

        // @todo - Cr5eating a new world here too, and also a node with this same world
        this->left_node = ; //this will be almost the copy of this node, with the same nodes, just starting with an empty states vector, and that it's pause_time will also not be there (ie. by default 0, meaning the world_node has an active world currently running)

    }

    void start_simulation(){    // causes the world to start running

        for( auto&& snake : this->world->snakes ){
            this->world->entity_threads.push_back(
                std::thread(&Snake::simulateExistence, snake)
            );
        }

        std::thread(&WorldPlot::start_auto_expansion(), this->world_plot);    // @future - Explore the possibilities of doing the world_plot expansion in this_thread, ie. the world's thread itself (that will likely have modifying the next while loop)

        // this loop `just waits AND ensures the world_plot has food avaialable`, (since the entities are on there own threads)
        while( this->world->_shared_concurrent_data.is_world_running() ){
            if( this->world->world_plot.food == nullptr ) this->world_plot.createFood(); //though this if block should NEVER be reached, since ateFood() also creates new Food
            this->captureState();   // can PAUSE then capture, for i am preferring some wrong coords in state, rather than stopping and starting threads again and again

            //pause for a unit time
            std::this_thread::sleep_for(std::chrono::milliseconds(statics::UNIT_TIME * 1000));
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


    public:
    const World_Ptr get_world() const{
        std::scoped_lock s(node_mutex);
        return &(this->data);
    }
    World_Node(World&) = delete;

    // @note - Be sure you have ALL respective arguments as taken by the World class constructor, since the node itself will need them to construct a new world
    World_Node(World_Ptr old_world, _timePoint t) : data(old_world, t){

        this->pause_time = data.currentTime;  //initially
    }

    private:
    World_Node() : pause_time(0), {
        this->world = new World();
    // create a new world here
    }

    friend class World_Tree;    //since it will be constructing the root node, ie. the first world

};

// it will actuall ystore dynamiucally allocated objects, not just there pointers
// template<typename node_data_type = World>  // @for_now - For now only, we are giving it this default templated version
class World_Tree : public Tree<World>{
    // typedef Tree_Node* Tree_Node_Ptr;
    typedef World_Node* World_Node_Ptr;
    // typedef _tree_node<World>* World_Node_Ptr;
    private:
    World_Node_Ptr root;
    // std::vector<Tree_Node_Ptr> all_nodes;    //vector of pointers to all nodes, this maybe additional storage, but gives ease destructing, and to know number of nodes
    int16_t num_nodes;

    struct {
        private:
        World_Node_Ptr __latest_world_node;  //pointer to the latest world currently running

        public:
        std::mutex tree_mutex;
        const World_Ptr get_latest_world(){
            std::scoped_lock s(tree_mutex);
            return __latest_world_node->get_world();
        }
        void update_node_time(){    // @note - Call this when a world is going to be stopped
            __latest_world_node->pause_time = __latest_world_node->get_world()->currentTime;
        }
    } _fast_access_data;    // temporary data for fast access, to currently running world

    public:
    bool initTree(){    //should be called after Verse::big_bang(), to initiate a world, and set it as the root node

    }

    World_Tree() : root(nullptr), num_nodes(0){}
    World_Tree(World& root_data) : root(new World_Node(root_data)), num_nodes(1){}
    World_Tree(World&& root_data) : root(new World_Node(root_data)), num_nodes(1){}
    ~World_Tree(){
        std::queue<World_Node_Ptr> qn;
        qn.push(root);
        World_Node_Ptr temp;

        while( !qn.empty() ){
            temp = qn.front();

            for( _tree_node<World>* i : temp->nodes ){
                qn.push(i);
            }
            delete qn.front();;
            qn.pop();
        }
    }
};
