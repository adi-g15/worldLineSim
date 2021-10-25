#pragma once

#include "entity.hpp"

#include <stack>

class Equation {
    /*
     * TODO
     *
     * IMPORTANT_NOTE: Currently using the equation (x^2 - 3x + 1) for any
     * equation passed, implement actual logic later on
     *
     * Internally use expression trees
     *
     * Currently only one variable
     */

  public:
    Equation(const std::string &eq_str);
    long eval(int x) const;
};

class Aliens : public Entity { // intentionally plural, i don't know if we
                               // should be classifying them as singular or
                               // plural (in group or not)
    /*
     * NOTE: Considering we are using fixed equations for positions, and fixed
     * position for blackholes, it can occur such that a particular equation's
     * graph NEVER touches that coordinate, ie. the alien will never be able to
     * get to the civilisation then, which is sad... FIX THIS
     *
     * Possible Solution: Maybe... make the blackhole coordinate variable... so
     * the aliens on it would just be 'travelling' places on the blackhole :D
     */

    std::vector<coord>
        blackhole_coords; // the resting destination for the aliens

    static inline std::stack<Aliens *>
        alien_civilisation; // pushed to this, once they reach the blackhole

    std::tuple<Equation, Equation, Equation>
        position_equations; // these three equations describe the position of
                            // the alien

    int current_var; // current variable used on which the position was
                     // evaluated, can be used by another entity to calculate
                     // where this alien is.... it won't share the current
                     // position with others directly... it's an alien, we have
                     // to find it :D

    bool should_travel{true};

  private:
    std::optional<Entity_Point> getPrimaryPos() const override;
    void _Action1() override {}
    void _Action2() override {}

  public:
    void simulateExistence() override;
    void pauseExistence() override;

    Aliens(World *);

    EntityState *_get_current_state() const override;
};
