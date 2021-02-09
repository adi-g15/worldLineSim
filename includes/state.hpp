#pragma once

#include "declarations.hpp"
#include "graphMat/direction.hpp"
#include <vector>

/*
The state should only be created when asked for, for on most cases, when world paused
*/
    // This is mainly for logging, hence not making it complex with Graph_Box* etc
struct State    // @idea Can later have food location in state too... not doind it now, it's up to `future` decision
{   
    std::vector < coord > curr_pos; //current positions of entities

    _timePoint state_time;

    private:
    State(
        _timePoint t
    ): state_time(t){}

    friend class World_Node;
    friend struct Log;
};
