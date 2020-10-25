#pragma once

#include "graphMat/direction.hpp"
#include <utility>
#include <cstdint>
#include <list>

#include "entity.hpp"

template<typename dimen_t = int32_t> // @assert - dimen_t must be integral type
struct _coord{
    dimen_t mX, mY;

    friend std::ostream& operator<<(std::ostream& os, const _coord<dimen_t>& coord){
        if(coord.mX >= 0){
            os << ' ';
            if(coord.mX < 10)
                os << ' ';
        }else if(coord.mX > -10)
            os << ' ';
        os << coord.mX <<',';
        if(coord.mY >= 0){
            os << ' ';
            if(coord.mY < 10)
                os << ' ';
        }else if(coord.mY > -10)
            os << ' ';

        os << coord.mY;
        return os;
    }

    _coord(dimen_t x, dimen_t y): mX(x), mY(y){}
};

typedef uint64_t _timePoint;
struct _box{    //Just as extension, to add more variables to the graph_box
    // since this will be the `data` content of the graph_box, so, it will be accessed by, this->data->varNameHere
        // only for INTERNAL USE, for path finding
    Direction __dir_came_from;  //stores the direction from which this node was reached

    std::list<Entity*> entities;    // @todo - Add any entity to the the box's entities vector which is entered by an entity

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

namespace statics
{
    // static constexpr std::pair< uint16_t, uint16_t > MAX_BOUNDS{ 1000, 1000 };    //applicable to ALL WORLDS CREATED
    // static constexpr std::pair< uint16_t, uint16_t > init_Max_Bound{ 40, 40 };    //applicable to A NEW WORLD
        // Currently using square matrix as parent, since it will be equally expanding later
    static constexpr uint16_t MAX_BOUND{ 1000 };    //applicable to ALL WORLDS CREATED
    static constexpr uint16_t init_Bound{ 40, };    //applicable to A NEW WORLD

    static constexpr float max_free_space = 1000.0f;    //so that space doesn't just get wasted
    static constexpr float min_free_space = 100.0f;    //if it falls below this, in a particular worlPlot, the expansion rate will be increased

    static constexpr _timePoint BIG_BANG_TIME = 0;    //can be time::now() later too
    static constexpr float UNIT_TIME = 1.0f;    //in terms of human world seconds

    // static constexpr int8_t init_expansion_speed{ 1 };   // @future - allow negative growths too
    static constexpr uint8_t init_expansion_speed{ 1 };

} // namespace statics

