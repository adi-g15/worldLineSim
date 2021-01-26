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

const coord& World::getFoodCoord()
{
    return world_plot.get_food().coordinate;
}

bool World::lockFood(const Snake*)
{
    if (!this->food_mutex.try_lock())   return false;

    this->food_mutex.lock();
    return true;
}

void World::ateFood(const Snake*){ //which snake ate it, log it, then randomize the food again
    //log the event, with snakeId

    // @todo

    // this->world_plot.reset_food();
    // this->createFood();
}

bool World::isCellEmpty(const Graph_Box_3D<Box>* c) const{

    if (!c) {
        throw std::invalid_argument("Graph_Box passed to isCellEmpty() MUST not be null");
    }
    // @todo -> Don't need this, remove it later, since with current graph matrix
    // return this->world_plot.isEmpty(c);
    return c->getData().entities.empty();
}

World::udimen_t World::getWorldDimen() const
{
    return this->world_plot.getOrder();
}

bool World::_RangeCheck(const coord_type& c) const{
    return this->world_plot.getOrder() > c.mX || this->world_plot.getOrder() > c.mY;
}

World::World(const World_Ptr world, _timePoint t) : currentTime(t), world_plot(this), path_finder(&world_plot){
    // @todo

    for( auto i = 0; i < this->_MAX_NumSnakes; i++ ){


    }


// @todo Log the event
// this->logs.push_back(Log(tmpState));

}

World::World() : currentTime(statics::BIG_BANG_TIME), world_plot(this), path_finder(&world_plot){
    // @todo

    // @todo - Initialise the world_plot here, or get it on another thread (not really required, can be on the same thread as of the world)
    // this->_WorldDimen = statics::init_Bound;

    this->snakes.reserve(this->_MAX_NumSnakes);
    for (auto i = 0; i < this->_MAX_NumSnakes; i++)
    {
        this->snakes.push_back( //later when simulated, let all snakes run on a different thread
            Snake{ this, util::Random::random<uint16_t>(2, 5) }
        );
    }

}

void World::getShortestPathToFood(const Entity_Point& origin, directionalPath& old_path) const{
    // @question -> Will std::forward with pointers work as well, (seems so, since it's just taking the value stored, which is the address in the pointer)
  // return this->world_plot.getShortestPathToFood( std::forward(origin) );
    return this->world_plot.getShortestPathToFood(origin, old_path);
}
