#include "Entities/star.hpp"

std::optional<Entity_Point> Star::getPrimaryPos() const
{
	return std::optional<Entity_Point>();
}

void Star::_Action1()
{
}

void Star::_Action2()
{
}

void Star::simulateExistence()
{
}

void Star::pauseExistence()
{
}

SolarState* Star::_get_current_state() const
{
	return nullptr;
}
