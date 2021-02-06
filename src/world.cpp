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

// Entities
#include "Entities/snake.hpp"
#include "Entities/rock.hpp"
#include "Entities/human.hpp"

// inlining this function gives Unresolved External Symbol to call to this EXACT function in World_Node's constructor
_timePoint World::getCurrentTime() const noexcept
{
    return this->world_plot.currentTime;
}

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

World::udimen_t World::getWorldDimen() const
{
    return this->world_plot.getOrder();
}

bool World::_RangeCheck(const coord_type& c) const{
    return this->world_plot.getOrder() > c.mX || this->world_plot.getOrder() > c.mY;
}

World::World(const World_Ptr world, _timePoint t) : world_plot(this, t), path_finder(&world_plot){
    // @todo

    for( auto i = 0; i < this->_MAX_NumSnakes; i++ ){


    }


// @todo Log the event
// this->logs.push_back(Log(tmpState));

}

// also starts the simulations for each snake executing
World::World() : simulationRunning(false), world_plot(this, statics::BIG_BANG_TIME), path_finder(&world_plot){
    // @todo

    this->entities.reserve(this->_MAX_NumSnakes);
    for (auto i = 0; i < this->_MAX_NumSnakes; i++)
    {
        this->entities.push_back(
            new Snake(this, util::Random::random<uint16_t>(5, 10))
        );
    }
    for (auto i = 0; i < 100; i++)
    {
        this->entities.push_back(
            new Rock(this)
        );
    }
    for (auto i = 0; i < 2; i++)
    {
        this->entities.push_back(
            new Human(this)
        );
    }

    //for (auto&& entity : this->entities) {
    //    switch (entity->type)
    //    {
    //    case Entity_Types::SNAKE:
    //        std::thread(&Snake::simulateExistence, entity).detach();
    //        break;
    //    case Entity_Types::ROCK:
    //        std::thread(&Rock::simulateExistence, entity).detach();
    //        break;
    //    case Entity_Types::HUMAN:
    //        std::thread(&Human::simulateExistence, entity).detach();
    //        break;
    //    }
    //}

    // COME HERE - Decide how is time incremented
    //while (this->simulationRunning)
    //{
    //    ++this->currentTime;
    //    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(statics::UNIT_TIME * 1000)));
    //}
}

World::~World()
{
    this->_shared_concurrent_data.reset_world_running();
    for (auto& entity : entities)
    {
        delete entity;
    }
    this->entities.clear();
    // the next loop isn't actually required, but can still be used
    //for (auto& snake_thread : entity_threads)
    //{
    //    if (snake_thread.joinable()) snake_thread.join();
    //}
    this->world_plot.pause_auto_expansion();
}

void World::getShortestPathToFood(const Entity_Point& origin, directionalPath& old_path) const{
    // @question -> Will std::forward with pointers work as well, (seems so, since it's just taking the value stored, which is the address in the pointer)
  // return this->world_plot.getShortestPathToFood( std::forward(origin) );
    return this->world_plot.getShortestPathToFood(origin, old_path);
}
