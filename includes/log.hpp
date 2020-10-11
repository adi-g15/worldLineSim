#pragma once

#include "state.hpp"

struct Log
{
    State currState;

    Log(State state): currState(state){}
};
