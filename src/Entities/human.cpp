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
	this->has_been_paused = false;

	while (this->should_wander && this->parent_world->is_world_running())
	{
		LOGGER::log_msg("Human Moving #{} from ({}, {}, {})", this->_id, this->curr_pos.point_coord.mX, this->curr_pos.point_coord.mY, this->curr_pos.point_coord.mZ);

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

	this->has_been_paused = true;
}

void Human::pauseExistence()
{
	if (!should_wander)	return;

	this->should_wander = false;
	while (!this->has_been_paused)	{}	// we keep checking until it's stopped
}

Human::Human(World_Ptr const world):
	Entity(Entity_Types::HUMAN),
	parent_world(world),
	curr_pos(nullptr, world->world_plot.getRandomCoord())
{
	LOGGER::log_msg("Human #{} aa gaya :D ... Location: ({}, {}, {})", this->_id, this->curr_pos.point_coord.mX, this->curr_pos.point_coord.mY, this->curr_pos.point_coord.mZ);

	this->curr_pos.graph_box = this->parent_world->get_box(curr_pos.point_coord);
	assert(curr_pos.graph_box != nullptr);	// remove this assert after tests written that getRandomCoord() always returns correct one
}

Human::Human(World_Ptr const world, const HumanState& prev_state):
	Entity(Entity_Types::HUMAN),
	parent_world(world),
	curr_pos(nullptr, prev_state.location)
{
	LOGGER::log_msg("Human #{} aa gaya :D ... Location: ({}, {}, {})", this->_id, this->curr_pos.point_coord.mX, this->curr_pos.point_coord.mY, this->curr_pos.point_coord.mZ);

	this->curr_pos.graph_box = this->parent_world->get_box(curr_pos.point_coord);
	assert(curr_pos.graph_box != nullptr);
}

Human::~Human()
{
	this->pauseExistence();
}

HumanState* Human::_get_current_state() const
{
	return new HumanState(this);
}

HumanState::HumanState(const Human* human):
	EntityState(Entity_Types::HUMAN),
	location(human->curr_pos.point_coord)
{}
