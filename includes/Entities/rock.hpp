#pragma once

#include "entity.hpp"

class Rock: public Entity {
	typedef World* World_Ptr;

public:
	const Entity_Point location;
	void _Action1() override {}
	void _Action2() override {}

	std::optional<Entity_Point> Entity::getPrimaryPos(void) const override { return this->location; }
	void simulateExistence() override {}

	Rock(World_Ptr);
	Rock(World_Ptr, const coord&);
};
