/*
    Get w = getWorld( t - t` )

    if( is_descendence(w, curr) ){
        par = w;
    }else {
        par = curr;
    }

    addNewWorld( par );
*/

#include "world.hpp"
#include "random.hpp"
#include "exceptions.hpp"
#include "util/math.hpp"

#include <algorithm>
#include <chrono>

void World::ateFood(const Snake*){ //which snake ate it, log it, then randomize the food again
    //log the event, with snakeId

    // @todo

    // this->food.reset();
    // this->createFood();
}

bool World::isCellEmpty(const coord_type& c) const{
    if( !this->_RangeCheck(c) )   return false;

    // @todo -> Don't need this, remove it later, since with current graph matrix
    // return this->world_plot.isEmpty(c);
}

bool World::_RangeCheck(const coord_type& c) const{
    return this->world_plot.getCurrentOrder() > c.mX || this->world_plot.getCurrentOrder() > c.mY;
}

World::World(const World_Ptr world, _timePoint t) : currentTime(t), currentState({}, this->currentTime), world_plot(this){
    // @todo

    for( auto i = 0; i < this->_MAX_NumSnakes; i++ ){


    }


    // run the next part on another thread, or let it be managed by the entities themselves
// starting entities to be done in constructor itself

// auto tmpState = State({}, this->currentTime);
// for(auto& snake: this->snakes){
//     snake.moveForward();
//     tmpState.curr_pos.push_back(snake.getHead());
// }

// @todo Log the event
// this->logs.push_back(Log(tmpState));

}

World::World() : currentTime(statics::BIG_BANG_TIME), currentState({}, this->currentTime), world_plot(this){
    // @todo

    // @todo - Initialise the world_plot here, or get it on another thread (not really required, can be on the same thread as of the world)
    // this->_WorldDimen = statics::init_Bound;

    this->snakes.reserve(this->_MAX_NumSnakes);
    this->snakes.push_back( //later when simulated, let all snakes run on a different thread
        Snake(this, Random::random(2, 5))
    );

    this->currentState = State({}, this->currentTime);

    this->start_WorldSimulation();

}

void World::start_WorldSimulation(){

    for( auto&& snake : this->snakes ){
        this->entity_threads.push_back(
            std::thread(&Snake::simulateExistence, snake)
        );
    }

    std::thread( &WorldPlot::start_auto_expansion(), this->world_plot );    // @future - Explore the possibilities of doing the world_plot expansion in this_thread, ie. the world's thread itself (that will likely have modifying the next while loop)

    // this loop `just waits AND ensures the world_plot has food avaialable`, (since the entities are on there own threads)
    while( this->_shared_concurrent_data.is_world_running() ){
        if( this->world_plot.food == nullptr ) this->world_plot.createFood(); //though this if block should NEVER be reached, since ateFood() also creates new Food

        //pause for a unit time
        std::this_thread::sleep_for(std::chrono::milliseconds(statics::UNIT_TIME * 1000));
    }

    this->stop_WorldSimulation(); //it will stop all the threads

    ++this->currentTime;

}

void World::stop_WorldSimulation(){
    this->_shared_concurrent_data._world_runnning = false;

    for (auto &&thread : this->entity_threads)
    {
        if( thread.joinable() )
            thread.join();
    }

    // @todo - Do whatever needs to be done, after all entities have been stopped
}

const World::dimen_t& World::get_curr_bound() const{
    return this->_curr_BOUND;
}

bool World::isPathClear(const Graph_Box<_box>* origin, const directionalPath& path) const{
    // return this->world_plot.isPathClear( std::forward(origin), std::forward(path) );
    return this->world_plot.isPathClear(origin, path);
}

directionalPath World::getShortestPathToFood(const Graph_Box<_box>* origin) const{
    // @question -> Will std::forward with pointers work as well, (seems so, since it's just taking the value stored, which is the address in the pointer)
  // return this->world_plot.getShortestPathToFood( std::forward(origin) );
    return this->world_plot.getShortestPathToFood(origin);
}

void World::getShortestPathToFood(const Graph_Box<_box>* origin, directionalPath& path) const{
    // return this->world_plot.getShortestPathToFood( std::forward(origin), path );
    return this->world_plot.getShortestPathToFood(origin, path);
}
