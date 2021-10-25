#pragma once

#include "entity.hpp"

class VidhayakJi : public Entity {
    coord curr_pos;
    World *parent_world;

    std::optional<Entity_Point> getPrimaryPos() const override {
        return std::nullopt;
    }
    void _Action1() override {}
    void _Action2() override {}

  public:
    bool is_jinda{false};

    void simulateExistence() override;
    void pauseExistence() override { this->is_jinda = false; }

    EntityState *_get_current_state()
        const override; // ONLY used for saving state, by World_Node

    VidhayakJi(World *);
};
