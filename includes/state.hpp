#pragma once

#include "declarations.hpp"
#include <vector>

struct State    // @idea Can later have food location in state too... not doind it now, it's up to `future` decision
{
    std::vector < _coord > curr_pos; //current positions of snakes

    _timePoint state_time;

    private:
    State(const std::vector < _coord >& pos, _timePoint t) : curr_pos(pos), state_time(t){}

    friend class World;
    friend class Log;

};
