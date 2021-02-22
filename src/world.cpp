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
#include "Entities/Custom/rabin.hpp"
#include "Entities/Custom/cheems_vidhayak.hpp"

bool World::is_world_running() const
{
    return _world_runnning.load();
}

void World::reset_world_running()
{
    this->_world_runnning.store(false);
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
    // Invariant - food_mutex was locked by this->lockFood()
    this->world_plot.createFood();  // resets and creates food
    this->food_mutex.unlock();
}

World::udimen_t World::get_world_dimen() const
{
    return this->world_plot.getOrder();
}

const coord& World::get_dimensions() const
{
    return this->world_plot._get_internal_dimensions();
}

Graph_Box_3D<Box>* World::get_box(const coord& pos)
{
    return this->world_plot.get_box(pos);
}

const Graph_Box_3D<Box>* World::get_box(const coord& pos) const
{
    return this->world_plot.get_box(pos);
}

void World::resume_simulation()
{
    this->_world_runnning.store(true);

    // stopping and freeing the entities
    for (auto& entity : this->entities)
    {
        std::thread(&Entity::simulateExistence, entity).detach();
    }

    // pause world from expanding
    this->world_plot.resume_auto_expansion();
}

void World::pause_simulation()
{
    // signal world plot, and entities, that they should stop expanding
    this->reset_world_running();

    // stopping and freeing the entities
    for (auto& entity : this->entities)
    {
        entity->pauseExistence();
    }

    // pause world from expanding
    this->world_plot.pause_auto_expansion();
}

void World::end_simulation()
{
    this->pause_simulation();

    for (auto& entity : this->entities)
    {
        delete entity;
    }
    this->entities.clear();
}

World::World(const State& start_state) :
    simulationRunning(true),
    world_plot(this, start_state),
    path_finder(&world_plot)
{
    for (const auto& entity_state : start_state.entity_states)
    {
        switch (entity_state->entity_type)
        {
        case Entity_Types::HUMAN :
            entities.push_back(new Human( this, *(static_cast<const HumanState*>(entity_state))) );
            break;
        case Entity_Types::ROCK :
            entities.push_back(new Rock(this, *(static_cast<const RockState*>(entity_state))));
            break;
        case Entity_Types::SNAKE :
            entities.push_back(new Snake(this, *(static_cast<const SnakeState*>(entity_state))));
            break;
        default:
            break;
        }
    }
}

// also starts the simulations for all entities
World::World():
    simulationRunning(false),
    world_plot(this),
    path_finder(&world_plot)
{
    this->entities.reserve(this->_Num_Snakes);
    int num_snakes = 1 + std::rand() % _Num_Snakes;
    for (auto i = 0; i < num_snakes; i++)
    {
        this->entities.push_back(
            new Snake(this)
        );
    }

    const int num_rocks = 1 + std::rand() % 52;
    for (auto i = 0; i < num_rocks; i++)
    {
        this->entities.push_back(
            new Rock(this)
        );
    }

    const int num_rabins = 3;
    for (auto i = 0; i < num_rabins; i++)
    {
        this->entities.push_back(
            new Rabin(this)
        );
    }

    const int num_cheems_doge = 3;
    for (auto i = 0; i < num_cheems_doge; i++)
    {
        this->entities.push_back(
            new VidhayakJi(this)
        );
    }

    bool gender_flag = false;
    for (auto i = 0; i < 2; i++)
    {
        this->entities.push_back(
            new Human(this, gender_flag ? Gender::MALE : Gender::FEMALE)
        );
        gender_flag = !gender_flag;
    }

    for (auto&& entity : this->entities) {
        std::thread(&Entity::simulateExistence, entity).detach();
    }
}

World::~World()
{
    this->reset_world_running();
    for (auto& entity : entities)
    {
        delete entity;
    }
    this->entities.clear();

    this->world_plot.pause_auto_expansion();
}

void World::getPathToFood(const Entity_Point& origin, directionalPath& old_path) const{
    // @question -> Will std::forward with pointers work as well, (seems so, since it's just taking the value stored, which is the address in the pointer)
    return this->world_plot.getShortestPathToFood(origin, old_path);
}
