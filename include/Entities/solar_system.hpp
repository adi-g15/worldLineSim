#pragma once

#include "Entities/planet.hpp"
#include "Entities/star.hpp"
#include "entity.hpp"

class Solar_System; // forward-decl

struct SolarSystemState : public EntityState {};

class Solar_System : Entity {
    void _Action1() override {}
    void _Action2() override {}

  public:
    static constexpr int mStars =
        1; //	@future - Support for multiple star systems, being at the center
           //(center region of the system at a particular time)
    static constexpr int mPlanets = 17;

    void simulateExistence() override;
    void pauseExistence() override;

    SolarSystemState *_get_current_state() const override;
};
