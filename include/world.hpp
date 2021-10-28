#pragma once

#include "Entities/snake.hpp"
#include "state.hpp"
#include "world_plot.hpp"

#include <mutex>
import <thread>;
import <unordered_map>;
import <utility>;
import <vector>;

typedef World *World_Ptr;
class World : public _ID {
  public:
    typedef statics::dimen_t dimen_t;
    typedef coord coord_type;

    typedef std::make_unsigned_t<dimen_t> udimen_t;
    WorldPlot world_plot;

    bool is_world_running() const;
    void reset_world_running();

    // @note - ateFood() is expected NEVER to fail, based on the food_mutex,
    // hence returns void
    std::mutex food_mutex; // locks JUST AFTER Snake::eatFood() starts and
                           // BEFORE World::ateFood(Snake*)
    const coord &getFoodCoord();
    bool lockFood(const Snake *);
    void ateFood(const Snake *); // which snake ate it, log it, then create the
                                 // food again

    //------constants------//
    static constexpr int _Num_Snakes = 10;
    static constexpr int _init_SnakeLength = 5;
    //------        ------//

    void getPathToFood(const Entity_Point &origin, directionalPath &) const;

    std::vector<Entity *> entities; // accessed by World_Plot::createFood()

    udimen_t get_world_dimen() const;
    const coord &get_dimensions() const;

    Graph_Box_3D<Box> *get_box(const coord &pos);
    const Graph_Box_3D<Box> *get_box(const coord &pos) const;

    void resume_simulation();
    void pause_simulation();
    void end_simulation();

    World();
    World(const State
              &prev_state); // the world object doesn't need to be linked to
                            // parent node, that's handled by World_Node

  private:
    bool simulationRunning;
    std::atomic_bool _world_runnning{
        true}; // world will keep moving forward (ie. entities will keep
               // existing and acting)

    Path_Finder path_finder;

    ~World();

    friend class World_Node;
};
