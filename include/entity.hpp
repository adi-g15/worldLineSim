#pragma once

#include <any>
#include <functional> // for std::reference_wrapper
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "declarations.hpp"
#include "entity_types.hpp"
#include "forward_decl.hpp"
#include "graphMat/graph_box.hpp" // for Graph_Box
#include "id_creator.hpp"

typedef void (*Action_Ptr)(void);

struct Entity_Point { // a general class, since each object will have at least 1
                      // POINT, for example for the snake this will be its head,
                      // for a planet, this can be its center, while a square
                      // may store 4 Entity_Point
    Graph_Box_3D<Box> *graph_box;

    coord point_coord;

    void reset(Graph_Box_3D<Box> *new_box, coord &new_coord) {
        this->graph_box = new_box;
        this->point_coord = new_coord;
    }

    Entity_Point(Graph_Box_3D<Box> *initial_box,
                 const coord &coordinate) noexcept
        : graph_box(initial_box), point_coord(coordinate) {}
};

struct EntityState {
    const Entity_Types entity_type;
    EntityState(Entity_Types type) : entity_type(type){};
};

class Entity : public _ID {
    typedef statics::dimen_t dimen_t;

  protected:
    // std::vector<Action_Ptr> supported_Operations;
    // std::vector<void (*)()> supported_Operations;
    // std::vector<void()> supported_Operations;   //same as above
  public:
    const Entity_Types type;
    // @warning -> Check for any additional copy being created due to use of
    // std::reference_wrapper::get() over the places in code
    virtual std::optional<Entity_Point>
    getPrimaryPos() const = 0; // optional since NOT mandatory that every entity
                               // will be having a entity_point
    virtual void _Action1() = 0; // only 2 actions supported as of now
    virtual void _Action2() = 0;

    // Simulation functions
    virtual void simulateExistence() = 0;
    virtual void pauseExistence() = 0;

    // Time Travel functions _ That allow the time travel feature
    virtual EntityState *
    _get_current_state() const = 0; // ONLY used for saving state, by World_Node

    Entity(Entity_Types type) : _ID(), type(type) {
        // supported_Operations.push_back( &foo );  //works, void(void) in
        // global scope supported_Operations.push_back( &Entity::_Action1 );
        // //doesn't work supported_Operations.push_back( &Entity::_Action1 );
        // //doesn't work supported_Operations = {&Entity::_Action1,
        // &Entity::_Action2};
    }

    friend class World;
};
