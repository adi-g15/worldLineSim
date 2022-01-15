#include "Entities/rock.hpp"
#include "logger.hpp"
#include "world.hpp"

Rock::Rock(World_Ptr world) : Rock(world, world->world_plot.getRandomCoord()) {}

Rock::Rock(World_Ptr world, const RockState &state)
    : Rock(world, state.location) {}

Rock::Rock(World_Ptr world, const coord &location)
    : Entity(Entity_Types::ROCK),
      location(world->world_plot[location], location) {
    assert(this->location.graph_box !=
           nullptr); // ie. we received valid coordinate, AND that coordinate is
                     // still in matrix

    LOGGER::log_trace("Rock #{} placed in world #{} at ({}, {}, {}}", this->_id,
                      world->_id, this->location.point_coord.mX,
                      this->location.point_coord.mY,
                      this->location.point_coord.mZ);

    this->location.graph_box->getDataRef().addEntity(this);
}

RockState *Rock::_get_current_state() const { return new RockState(this); }

RockState::RockState(const Rock *rock)
    : EntityState(Entity_Types::ROCK), location(rock->location.point_coord) {}
