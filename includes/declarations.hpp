#pragma once

#include <ostream>
#include <utility>
#include <cstdint>
#include <cassert>
#include <forward_list>

#include "graphMat/direction.hpp"
#include "graphMat/../../util/coord.hpp"    // not recommended like this, @me @note - Update when Utilities become a library of themselves
#include "util/ranges.hpp"  // for util::contains function

#include "config.hpp"

typedef util::_coord3D<int32_t> coord;
class Entity;   // forward decl

struct Box{    //Just as extension, to add more variables to the graph_box
//    Direction __dir_came_from;  //stores the direction from which this node was reached

    const Entity* entity{nullptr};
    //std::forward_list<const Entity*> entities;    // @todo - Add any entity to the the box's entities vector which is entered by an entity

public:
    bool hasEntities() const noexcept { return  this->entity != nullptr; }
    //inline void addEntity(const Entity* entity) {
    inline void addEntity(const Entity* entity) {
        if(this->entity == nullptr) this->entity = entity;
        //entities.emplace_front(entity);
    }
    inline void remEntity(const Entity* entity) noexcept {   // noexcept gaurantee, if not found then no action
        //entities.remove(entity);
        if (this->entity == entity)  this->entity = nullptr;

//#ifdef DEBUG
//        assert(std::find(std::cbegin(entities), std::cend(entities), entity) != entities.cend());
//        "The box currently doesn't hold any reference to the entity"
//#endif // DEBUG
    }

    Box() noexcept {}
    // bool isEmpty() const noexcept {return entities.empty();}    //maybe removed
};

enum Event{  //for logging puposes
    // entity events
    Entity_Created = 0,
    Entity_Destroyed,
    Entity_Move,
    Entity_Grow,

    // world events
    World_Created,
    World_Destroyed,
    World_Stopped_Expanding,
    World_Expanding,
    World_Paused,
    World_Resume,   //won't be used now

    // verse events
    DESTRUCTION_START,

    // graphMat
    //GraphMat_Allocation
};

namespace logger {
    constexpr std::array<int, 8> verbosity_levels{  // in order of declarations in Event
        0,
        0,
        2,
        1,

        -1,
        -1,
        -1,

        -2
    };
}
