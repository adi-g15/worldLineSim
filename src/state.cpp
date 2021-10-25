#include "state.hpp"
#include "world.hpp"
#include "world_node.hpp"

State::State(World_Node *world_node)
    : state_time(world_node->getCurrentTime()),
      universe_order(world_node->get_world().get_world_dimen()),
      food_pos(world_node->get_world().world_plot.get_food().coordinate) {
    entity_states.reserve(world_node->get_world().entities.size());
    for (const auto &entity : world_node->get_world().entities) {
        entity_states.push_back(entity->_get_current_state());
    }
}

// State::~State()
//{
//	LOGGER::log_msg("Removing state for time {}", this->state_time);
//// always removing the 1st state for some reason, couldn't know till now,
// what's causing for the destructor to be called, since it's stored simply as
// vector<State>, upon that why only the 1st element for the first 3 times
// atleast (after that invalid access error) 	for (auto* state :
// entity_states)
//	{
//		delete state;
//	}
//	entity_states.clear();
//}
