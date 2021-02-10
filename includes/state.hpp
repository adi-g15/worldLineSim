#pragma once

#include "entity.hpp"
#include "graphMat/direction.hpp"
#include <vector>

class World_Node;   // forward-decl

/*
The state should only be created when asked for, for on most cases, when world paused
*/
    // This is mainly for logging, hence not making it complex with Graph_Box* etc
struct State    // @idea Can later have food location in state too... not doind it now, it's up to `future` decision
{
    std::vector< Entity_Types > types;
    std::vector < EntityState > entity_states; //current positions of entities

    const _timePoint state_time;
    const statics::dimen_t universe_order;

    State(World_Node*);

};
