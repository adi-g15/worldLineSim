#include "Entities/human.hpp"
#include "world.hpp"
#include "graphMat/iterators.hpp"

std::optional<Entity_Point> Human::getPrimaryPos() const
{
	return this->curr_pos;
}

void Human::simulateExistence()
{
	this->should_wander = true;

	while (this->should_wander)
	{
		LOGGER::log_it(this->parent_world->_id, Event::Entity_Move/*, "Human Moving #%d from ", this->_id, this->curr_pos.point_coord*/);

		graphMat::NeighbourIterator<Box> iter(this->curr_pos.graph_box);

		for (; iter ; ++iter)
		{
			auto box_ptr = iter.operator->();
			if ( ! iter->getData().hasEntities()) {
				this->curr_pos.graph_box = box_ptr;
				coord increment_coord = iter._getIncrementCoords();

				if (curr_pos.graph_box != iter.center_box) {
					// if it's not the center box we passed to initialiser, then it is EITHER FRONT_FACING or BACK_FACING
					increment_coord.mZ = (curr_pos.graph_box->FRONT_FACING == iter.center_box) ? 1 : -1;
				}

				this->curr_pos.point_coord += increment_coord;
				break;
			}
		}

		std::this_thread::sleep_for(std::chrono::seconds( static_cast<int>( statics::UNIT_TIME * TIME_DIFF_PER_MOVE )));
	}
}

void Human::pauseExistence()
{
	this->should_wander = false;
}

Human::Human(World_Ptr const world):
	Entity(Entity_Types::HUMAN),
	parent_world(world),
	curr_pos(nullptr, world->world_plot.getRandomCoord())
{
	LOGGER::log_it(this->parent_world->_id, Event::Entity_Created/*, "Human aa gaya :D ... Location: ", this->curr_pos.point_coord*/);

	this->curr_pos.graph_box = this->parent_world->get_box(curr_pos.point_coord);
	assert(curr_pos.graph_box != nullptr);	// remove this assert after tests written that getRandomCoord() always returns correct one
}

EntityState Human::_get_current_state() const
{
	return HumanState(this);
}

HumanState::HumanState(const Human* human):
	EntityState(Entity_Types::HUMAN),
	location(human->curr_pos.point_coord)
{}
