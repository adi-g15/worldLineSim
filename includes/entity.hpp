#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <any>
#include <optional>
#include <functional>   // for std::reference_wrapper

#include "declarations.hpp"
#include "id_creator.hpp"
#include "graphMat/3d_graph_box.hpp"   // for Graph_Box
#include "logger.hpp"

typedef void (*Action_Ptr)(void);

struct Prop{    // @future - See the @future comment on the Entity::getUniqProp() function
    std::any data;
};

struct SnakeProp: Prop{
    // int data;   // for snake, the unique property (ie. this `data`) will be an int, ie. length
};

enum class Entity_Types {
    SNAKE,
    ROCK,
    HUMAN
    // more can be added later, it's made to be expandible, though do see the member declarations as for what ANY entity must have
};

struct Entity_Point {    // a general class, since each object will have at least 1 POINT, for example for the snake this will be its head, for a planet, this can be its center, while a square may store 4 Entity_Point
    Graph_Box_3D<Box>* graph_box;

    coord point_coord;

    void reset(Graph_Box_3D<Box>* new_box, coord& new_coord) {
        this->graph_box = new_box;
        this->point_coord = new_coord;
    }

    Entity_Point(Graph_Box_3D<Box>* initial_box, const coord& coordinate) noexcept: graph_box(initial_box), point_coord(coordinate) {}
};

class Entity: public _ID{
    typedef statics::dimen_t dimen_t;
protected:
    //std::vector<Action_Ptr> supported_Operations;
    // std::vector<void (*)()> supported_Operations;
    // std::vector<void()> supported_Operations;   //same as above
public:
    const Entity_Types type;
    // @warning -> Check for any additional copy being created due to use of std::reference_wrapper::get() over the places in code
    virtual std::optional<Entity_Point> getPrimaryPos() const = 0;  // optional since NOT mandatory that every entity will be having a entity_point
    virtual void _Action1() = 0;    //only 2 actions supported as of now
    virtual void _Action2() = 0;

    virtual void simulateExistence() = 0;   // simulate the work when on a single thread
    virtual void pauseExistence() = 0;

    Entity(Entity_Types type):
        _ID(),
        type(type)
    {
        // supported_Operations.push_back( &foo );  //works, void(void) in global scope
        // supported_Operations.push_back( &Entity::_Action1 ); //doesn't work
        // supported_Operations.push_back( &Entity::_Action1 ); //doesn't work
        // supported_Operations = {&Entity::_Action1, &Entity::_Action2};
    }

    friend class World;
};
