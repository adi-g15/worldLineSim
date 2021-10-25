#pragma once

#include "entity.hpp"

#include <stack>

class Mathematician : public Entity {

    /*
     * DESIGN FLAW : static data will get shared with other universes too, DONT
     * DO IT, don't use static when can be
     */
    static inline std::stack<coord> shared_knowledge;
    std::string name;

    std::optional<Entity_Point> getPrimaryPos() const override;
    void _Action1() override {}
    void _Action2() override {}

    bool should_research{true};

    // Simulation functions
  public:
    Mathematician(World *);

    void simulateExistence() override;
    void pauseExistence() override;

    // Time Travel functions _ That allow the time travel feature
    EntityState *_get_current_state() const override;
};
