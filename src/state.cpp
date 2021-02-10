#include "state.hpp"
#include "world.hpp"

State::State(World* world):
	state_time(world->getCurrentTime()),
	universe_order(world->getWorldDimen())
{
	//this->entity_states.reserve(world->entities.size());
	//for (const auto&& entity : world->entities) {
	//	if (entity->getPrimaryPos().has_value()) {
	//		this->entity_states.push_back(entity->getPrimaryPos().value().point_coord);    // snakes will always have a primary position
	//	}
	//}
}
