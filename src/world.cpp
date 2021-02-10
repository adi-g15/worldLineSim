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
//#include "Entities/Custom/rabin.hpp"
//#include "Entities/Custom/cheems_vidhayak.hpp"

_timePoint World::getCurrentTime() const noexcept
{
    return this->world_plot.currentTime;
}

const coord& World::getFoodCoord()
{
    return world_plot.get_food().coordinate;
}

bool World::lockFood(const Snake* snake)
{
    return (
        snake->getHead().point_coord == this->getFoodCoord()
        &&
        this->food_mutex.try_lock()
    );
}

void World::ateFood(const Snake* snake){ //which snake ate it, log it, then randomize the food again
    // @log the event, with snakeId

    // Invariant - food_mutex was locked by this->lockFood()
    this->world_plot.createFood();  // resets and creates food
    this->food_mutex.unlock();
}

World::udimen_t World::getWorldDimen() const
{
    return this->world_plot.getOrder();
}

const coord& World::get_dimensions() const
{
    return this->world_plot._get_internal_dimensions();
}

bool World::_RangeCheck(const coord_type& c) const{
    return this->world_plot.getOrder() > c.mX || this->world_plot.getOrder() > c.mY;
}

void World::capture_state() {
    //State state{ this->world_plot.currentTime }; // we will set state only once, further calls to this function MUST NOT be made

}

World::World(const World_Ptr world, _timePoint t) : world_plot(this, t), path_finder(&world_plot){
    // @todo

    //const State latest_state = world->capture_state();  // since the previous world will keep running, so we `capture` it's current state to create our new world

    for( auto i = 0; i < this->_MAX_NumSnakes; i++ ){


    }


// @todo Log the event
// this->logs.push_back(Log(tmpState));

}

// also starts the simulations for all entities
World::World() : simulationRunning(false), world_plot(this, statics::BIG_BANG_TIME), path_finder(&world_plot){
    this->entities.reserve(this->_MAX_NumSnakes);
    for (auto i = 0; i < 0; i++)
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

    for (auto&& entity : this->entities) {
        std::thread(&Entity::simulateExistence, entity).detach();
    }
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
    return this->world_plot.getShortestPathToFood(origin, old_path);
}
