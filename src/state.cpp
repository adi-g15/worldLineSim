#include "state.hpp"
#include "world.hpp"
#include "world_node.hpp"

State::State(World_Node* world_node):
	state_time(world_node->getCurrentTime()),
	universe_order(world_node->get_world().get_world_dimen()),
	food_pos(world_node->get_world().world_plot.get_food().coordinate)
{
	entity_states.reserve(world_node->get_world().entities.size());
	for (const auto& entity : world_node->get_world().entities) {
		entity_states.push_back(entity->_get_current_state());
	}
}
