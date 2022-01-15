#pragma once

#include "declarations.hpp"
#include "forward_decl.hpp"
#include "graphMat/cube_mat.hpp"
#include "path_finder.hpp"
#include "state.hpp"
#include <atomic> // For std::atomic_bool (actually not mandatory, some extra loops are okay in auto_expand)

using Graph_Box = Graph_Box_3D<Box>;

struct Food {
    const Graph_Box *box{nullptr}; // @future - food may have different
                                   // points/nutitional values too
    coord coordinate; // food and entities will be having coordinates

  private:
    static coord get_new_food_pos(const std::vector<coord> &entity_pos);
    void reset(Food &&food);

    Food() = default; // CAUTION -> box may be nullptr
    Food(const Graph_Box *b, const coord &c) : box(b), coordinate(c) {}
    Food(Food &) = default;
    Food(Food &&) = default;
    friend class WorldPlot;
    friend class World;
};

// @future - If it lives on its own thread, put a auto expansion logic, that
// sleeps for 1 time unit, and then calls auto_expand()
class WorldPlot : public Cube_Matrix<Box> {
  public:
    typedef int32_t dimen_t;
    typedef World *World_Ptr;
    typedef Graph_Box graph_box_type;

  private:
    const World_Ptr parent_world;
    Path_Finder path_finder;
    Food food; // @future - food may have different points/nutitional values too

    void auto_expansion(); // keeps expanding TILL expansion_flag is TRUE
    void expand_once();
    void resume_auto_expansion();
    void pause_auto_expansion();

    float getFreeSpaceRatio() const;
    graph_box_type *get_box(const coord &position) {
        return this->operator[](position);
    }
    const graph_box_type *get_box(const coord &position) const {
        return this->operator[](position);
    }
    directionalPath getShortestPathToFood(const Entity_Point &origin) const;

  public:
    const util::_coord3D<int> &_get_internal_dimensions() const;

    const auto &get_food() const { return this->food; }
    coord getRandomCoord() const noexcept;

    void createFood();
    void createFood(
        const coord &food_pos); // this overload is utilised when worlds diverge

    void _rand_once_createFood(); // randomly creates food, only for the initial
                                  // moments where the entities may not be in
                                  // existence currently fully
    void _fit_coord_in_range(coord &) const noexcept;
    bool _is_in_range_coord(const coord &) const noexcept;

    const graph_box_type *return_nearby_empty_box(const coord &box_coord) const;

    bool isPathClear(const graph_box_type *origin,
                     const directionalPath &path) const;

    // @future - For optimising purpose, use the food coords in parent_world
    // (eg. to go search the direction which has the food, for eg, if it is in a
    // coord on right, only iterate through those)
    void getShortestPathToFood(const Entity_Point &origin,
                               directionalPath &) const;

    // By itself, worldplot only knows it's size and food position
    WorldPlot(const World_Ptr);
    WorldPlot(const World_Ptr, const State &start_state);

    friend class World;
    friend class Path_Finder; // it will be a friend of world_plot too, so as to
                              // control its ability to auto expand
};
