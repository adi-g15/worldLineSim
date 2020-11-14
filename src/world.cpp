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
#include "util/random.hpp"
#include "exceptions.hpp"
#include "util/math.hpp"

#include <algorithm>
#include <chrono>

void World::ateFood(const Snake*){ //which snake ate it, log it, then randomize the food again
    //log the event, with snakeId

    // @todo

    // this->world_plot.reset_food();
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

World::World(const World_Ptr world, _timePoint t) : currentTime(t), world_plot(this){
    // @todo

    for( auto i = 0; i < this->_MAX_NumSnakes; i++ ){


    }


// @todo Log the event
// this->logs.push_back(Log(tmpState));

}

World::World() : currentTime(statics::BIG_BANG_TIME), world_plot(this){
    // @todo

    // @todo - Initialise the world_plot here, or get it on another thread (not really required, can be on the same thread as of the world)
    // this->_WorldDimen = statics::init_Bound;

    this->snakes.reserve(this->_MAX_NumSnakes);
    this->snakes.push_back( //later when simulated, let all snakes run on a different thread
        Snake{this, util::Random::random(2, 5)}
    );

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
