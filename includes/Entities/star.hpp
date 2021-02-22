#pragma once

#include "entity.hpp"

class Star;	//forward-decl
class SolarSystem;	//forward-decl

struct SolarState : public EntityState {

};

/// <summary>
///	@note - This class is meant to also store
///
/// @future-  Make it capable enough to hold densities and calculate the physics equations for them
/// </summary>
class Star : public Entity {
	//float matter_density;

	SolarSystem* parent_system;
public:
	std::optional<Entity_Point> getPrimaryPos() const override;
	void _Action1() override;
	void _Action2() override;
	void simulateExistence() override;
	void pauseExistence() override;
	SolarState* _get_current_state() const override;
};
