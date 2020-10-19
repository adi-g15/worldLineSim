#pragma once

#include <utility>
#include <cstdint>
#include <list>

#include "entity.hpp"

template<typename dimen_t = int32_t>
struct _coord{
    dimen_t mX, mY;

    _coord(dimen_t x, dimen_t y): mX(x), mY(y){}
};

typedef uint64_t _timePoint;

struct _box{    //Just as extension, to add more variables to the graph_box
    // since this will be the `data` content of the graph_box, so, it will be accessed by, this->data->varNameHere
        // only for INTERNAL USE, for path finding
    Direction __dir_came_from;  //stores the direction from which this node was reached

    std::list<Entity*> entities;

    // bool isEmpty = true;    //maybe removed
};

namespace statics
{
    static constexpr std::pair< uint16_t, uint16_t > MAX_BOUNDS{ 1000, 1000 };    //applicable to ALL WORLDS CREATED
    static constexpr std::pair< uint16_t, uint16_t > init_Max_Bound{ 40, 40 };    //applicable to A NEW WORLD

    static constexpr _timePoint BIG_BANG_TIME = 0;    //can be time::now() later too
    static constexpr float UNIT_TIME = 1.0f;    //in terms of human world seconds

} // namespace statics

