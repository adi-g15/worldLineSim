#pragma once

#include "state.hpp"

struct Log
{
    const State currState;

    Log(State state): currState(state){}
};
