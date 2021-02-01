#pragma once

#include <ostream>
#include <utility>
#include <cstdint>
#include <cassert>
#include <forward_list>

#include "graphMat/direction.hpp"
#include "graphMat/util/coord.hpp"
#include "util/ranges.hpp"  // for util::contains function

#include "config.hpp"

typedef util::_coord3D<int32_t> coord;
class Entity;   // forward decl

struct Box{    //Just as extension, to add more variables to the graph_box
//    Direction __dir_came_from;  //stores the direction from which this node was reached

    std::forward_list<const Entity*> entities;    // @todo - Add any entity to the the box's entities vector which is entered by an entity

public:
    bool hasEntities() const noexcept { return  !this->entities.empty(); }
    inline void addEntity(const Entity* entity) {  // made const, so that can add remove entities even through const getter in worldPlot
        entities.emplace_front(entity);
    }
    inline void remEntity(const Entity* entity) noexcept {   // noexcept gaurantee, if not found then no action
        entities.remove(entity);

#ifdef DEBUG
        assert(std::find(std::cbegin(entities), std::cend(entities), entity) != entities.cend());
        "The box currently doesn't hold any reference to the entity"
#endif // DEBUG
    }

    Box() noexcept {}
    // bool isEmpty() const noexcept {return entities.empty();}    //maybe removed
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
