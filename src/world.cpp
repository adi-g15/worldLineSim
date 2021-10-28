#include "world.hpp"
#include "db/database.hpp"
#include "exceptions.hpp"
#include "util/math.hpp"
#include "util/random.hpp"

#include <algorithm>
import <chrono>;

// Entities
#include "Entities/Custom/aliens.hpp"
#include "Entities/Custom/cheems_vidhayak.hpp"
#include "Entities/Custom/mathematicians.hpp"
#include "Entities/Custom/rabin.hpp"
#include "Entities/human.hpp"
#include "Entities/rock.hpp"
#include "Entities/snake.hpp"

// Meta Entities
#include "Entities/Meta/Custom/GDSC/gdsc.hpp"

bool World::is_world_running() const { return _world_runnning.load(); }

void World::reset_world_running() { this->_world_runnning.store(false); }

const coord &World::getFoodCoord() { return world_plot.get_food().coordinate; }

bool World::lockFood(const Snake *snake) {
    return (snake->getHead().point_coord == this->getFoodCoord() &&
            this->food_mutex.try_lock());
}

void World::ateFood(const Snake *snake) { // which snake ate it, log it, then
                                          // randomize the food again
    // Invariant - food_mutex was locked by this->lockFood()
    this->world_plot.createFood(); // resets and creates food
    this->food_mutex.unlock();
}

World::udimen_t World::get_world_dimen() const {
    return this->world_plot.getOrder();
}

const coord &World::get_dimensions() const {
    return this->world_plot._get_internal_dimensions();
}

Graph_Box_3D<Box> *World::get_box(const coord &pos) {
    return this->world_plot.get_box(pos);
}

const Graph_Box_3D<Box> *World::get_box(const coord &pos) const {
    return this->world_plot.get_box(pos);
}

void World::resume_simulation() {
    this->_world_runnning.store(true);

    // stopping and freeing the entities
    for (auto &entity : this->entities) {
        std::thread(&Entity::simulateExistence, entity).detach();
    }

    // pause world from expanding
    this->world_plot.resume_auto_expansion();
}

void World::pause_simulation() {
    // signal world plot, and entities, that they should stop expanding
    this->reset_world_running();

    // stopping and freeing the entities
    for (auto &entity : this->entities) {
        entity->pauseExistence();
    }

    // pause world from expanding
    this->world_plot.pause_auto_expansion();
}

void World::end_simulation() {
    this->pause_simulation();

    for (auto &entity : this->entities) {
        delete entity;
    }
    this->entities.clear();
}

World::World(const State &start_state)
    : simulationRunning(true), world_plot(this, start_state),
      path_finder(&world_plot) {
    for (const auto &entity_state : start_state.entity_states) {
        switch (entity_state->entity_type) {
        case Entity_Types::HUMAN:
            entities.push_back(new Human(
                this, *(static_cast<const HumanState *>(entity_state))));
            break;
        case Entity_Types::ROCK:
            entities.push_back(new Rock(
                this, *(static_cast<const RockState *>(entity_state))));
            break;
        case Entity_Types::SNAKE:
            entities.push_back(new Snake(
                this, *(static_cast<const SnakeState *>(entity_state))));
            break;
        default:
            break;
        }
    }
}

// also starts the simulations for all entities
World::World()
    : simulationRunning(false), world_plot(this), path_finder(&world_plot) {
    this->entities.reserve(this->_Num_Snakes);
    int num_snakes = 1 + std::rand() % _Num_Snakes;
    for (auto i = 0; i < num_snakes; i++) {
        this->entities.push_back(new Snake(this));
    }

    const int num_rocks = 1 + std::rand() % 52;
    for (auto i = 0; i < num_rocks; i++) {
        this->entities.push_back(new Rock(this));
    }

    const int num_rabins = 1;
    for (auto i = 0; i < num_rabins; i++) {
        this->entities.push_back(new Rabin(this));
    }

    const int num_cheems_doge = 1;
    for (auto i = 0; i < num_cheems_doge; i++) {
        this->entities.push_back(new VidhayakJi(this));
    }

    bool gender_flag = false;
    const uint32_t num_humans = std::rand() % 5 + 2;
    for (auto i = 0; i < num_humans; i++) {
        this->entities.push_back(
            new Human(this, gender_flag ? Gender::MALE : Gender::FEMALE));
        gender_flag = !gender_flag;
    }

    const uint32_t num_aliens = 1 /*std::rand() % 3*/;
    for (auto i = 0; i < num_aliens; i++) {
        this->entities.push_back(new Aliens(this));
    }

    auto /*const*/ *this_verse = this;
    const uint32_t num_mathematicians = 1;
    for (auto i = 0; i < num_mathematicians; ++i) {
        this->entities.push_back(new Mathematician(this));
    }

    // @adi TODO Abhi ke liye members idhar pass karna adi, dynamically change
    // bhi kr skte lekin file wala achha dikhega auto member_details_yaml =
    // util::read_yaml( std::string(statics::DATABASE_DIRNAME) +
    // "/gdsc_nitp_members.yaml" );
    auto *gdsc_nit_patna = new GDSC(
        this, "NIT Patna - GDSC",
        std::map<std::string, Member>({
            {"Divya Prakash",
             Member(this_verse, "Divya Prakash", Gender::MALE, "Lead", 0.8)},
            {"Ashwini",
             Member(this_verse, "Ashwini", Gender::MALE, "Topper", 0.4, 0.7)},
            {"Asmit", Member(this_verse, "Asmit", Gender::MALE, "Comder")},
            {"Neeraj",
             Member(this_verse, "Neeraj", Gender::MALE, "Big Fan", 0.8, 0.8)},
            {"Palak", Member(this_verse, "Palak", Gender::FEMALE, "TagLearner",
                             0.8, 0.7)},
            {"Suraj", Member(this_verse, "Suraj", Gender::MALE, "Suraj")},
            {"Srijan", Member(this_verse, "Srijan", Gender::MALE, "Sri")},
            {"Pragati.",
             Member(this_verse, "Pragati##", Gender::FEMALE, "Core")},
            {"Pragati..",
             Member(this_verse, "Pragati@@", Gender::FEMALE, "Core")},
            {"Amrit", Member(this_verse, "Amrit", Gender::MALE, "Core")},
            {"Shagufta",
             Member(this_verse, "Shagufta", Gender::FEMALE, "Core")},
            {"Uddeshya", Member(this_verse, "Uddeshya", Gender::MALE,
                                "Designer", 0.6, 0.7)},
            {"Priyanshu",
             Member(this_verse, "Priyanshu", Gender::MALE, "Core")},
            {"Paritosh", Member(this_verse, "Paritosh", Gender::MALE, "...")},
            {"Rachit", Member(this_verse, "Rachit", Gender::MALE, "Core")},
            {"Shubhangi",
             Member(this_verse, "Shubhangi", Gender::FEMALE, "Core")},
            {"Shivam Jha", Member(this_verse, "Shivam", Gender::MALE, "Core")},
            {"Rishabh Sir Big Fan",
             Member(this_verse, "Rishabh", Gender::MALE, "Fan holder")},
            {"Animesh", Member(this_verse, "Animesh", Gender::MALE, "Maru")},
            {"Raj", Member(this_verse, "Raj", Gender::MALE, "Core")},
            {"Pratik", Member(this_verse, "Pratik", Gender::MALE, "Core")},
            {"Digvijay", Member(this_verse, "Digvijay", Gender::MALE, "Core")},
            {"Samridhhi",
             Member(this_verse, "Samridhhi", Gender::FEMALE, "Core")},
            {"Tess", Member(this_verse, "Tejas", Gender::FEMALE, "Manhas")},
        }));
    this->entities.push_back(gdsc_nit_patna);

    for (auto &&entity : this->entities) {
        std::thread(&Entity::simulateExistence, entity).detach();
    }
}

World::~World() {
    this->reset_world_running();
    for (auto &entity : entities) {
        delete entity;
    }
    this->entities.clear();

    this->world_plot.pause_auto_expansion();
}

void World::getPathToFood(const Entity_Point &origin,
                          directionalPath &old_path) const {
    // @question -> Will std::forward with pointers work as well, (seems so,
    // since it's just taking the value stored, which is the address in the
    // pointer)
    return this->world_plot.getShortestPathToFood(origin, old_path);
}
