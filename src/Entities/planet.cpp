#include "Entities\planet.hpp"

std::optional<Entity_Point> Planet::getPrimaryPos() const
{
	return std::optional<Entity_Point>();
}

void Planet::_Action1()
{
}

void Planet::_Action2()
{
}

void Planet::simulateExistence()
{
}

void Planet::pauseExistence()
{
}

PlanetState* Planet::_get_current_state() const
{
	return nullptr;
}
