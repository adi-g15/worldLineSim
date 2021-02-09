#pragma once

#include "entity.hpp"

class VidhayakJi : public Entity {
    bool is_jinda{ false };
    coord curr_pos;
    World* parent_world;

    std::optional<Entity_Point> getPrimaryPos() const override { return std::nullopt; }
    void _Action1() override {}
    void _Action2() override {}

public:
    void simulateExistence() override;
    void pauseExistence() override {
        this->is_jinda = false;
    }

    VidhayakJi(World*);
};
