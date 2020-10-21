#pragma once

#include "declarations.hpp"
#include "graphMat/direction.hpp"
#include <vector>

    // This is mainly for logging, hence not making it complex with Graph_Box* etc
struct State    // @idea Can later have food location in state too... not doind it now, it's up to `future` decision
{
    typedef _coord<int32_t> coord_type;
    std::vector < coord_type > curr_pos; //current positions of snakes

    _timePoint state_time;

    private:
    State(const std::vector < coord_type >& pos, _timePoint t) : curr_pos(pos), state_time(t){}

    friend class World;
    friend struct Log;

};
