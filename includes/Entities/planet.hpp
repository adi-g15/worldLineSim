#pragma once

#include "entity.hpp"

class Planet;	//forward-decl

struct PlanetState : public EntityState {

};

class Planet : public Entity {
	std::optional<Entity_Point> getPrimaryPos() const override;
	void _Action1() override;
	void _Action2() override;
	void simulateExistence() override;
	void pauseExistence() override;
	PlanetState* _get_current_state() const override;
};
