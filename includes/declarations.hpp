#pragma once

#include <ostream>
#include <utility>
#include <cstdint>
#include <list>

#include "graphMat/direction.hpp"
#include "util/coord.hpp"

#include "config.hpp"

typedef util::_coord<int32_t> coord;
class Entity;   // forward decl

struct _box{    //Just as extension, to add more variables to the graph_box
    // since this will be the `data` content of the graph_box, so, it will be accessed by, this->data->varNameHere
        // only for INTERNAL USE, for path finding
//    Direction __dir_came_from;  //stores the direction from which this node was reached

    std::list<Entity*> entities;    // @todo - Add any entity to the the box's entities vector which is entered by an entity

    public:
        bool hasEntities() const{    return  ! this->entities.empty(); }

    // bool isEmpty = true;    //maybe removed
};

enum class Event{  //for logging puposes
    ENTITY_BIRTH,
    ENTITY_DEATH,
    ENTITY_MOVE,
    ENTITY_GROW,
    NEW_WORLD,
    WORLD_PAUSE,
    // WORLD_RESUME,   //won't be used now
        //BIG_BANG happens only once, so not must to include
};
