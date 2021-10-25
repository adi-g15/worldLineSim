#pragma once

#include "entity.hpp"

class World; // forward-declaration
class Rock;  // forward-decl for RockState

struct RockState : public EntityState {
    const coord location;
    RockState(const Rock *);
};

class Rock : public Entity {
    typedef World *World_Ptr;

  public:
    const Entity_Point location;
    void _Action1() override {}
    void _Action2() override {}

    std::optional<Entity_Point> getPrimaryPos(void) const override {
        return this->location;
    }
    void simulateExistence() override {}
    void pauseExistence() override {}

    RockState *_get_current_state() const override;

    Rock(World_Ptr);
    Rock(World_Ptr, const coord &);
    Rock(World_Ptr, const RockState &);
};
