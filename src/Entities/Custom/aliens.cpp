#include "Entities/Custom/aliens.hpp"

#include "logger.hpp"
#include "world.hpp"
#include <chrono>
#include <thread>

Equation::Equation(const std::string &eq_str) {
    // TODO
}

long Equation::eval(int x) const {
    // TODO
    return (x * x - 3 * x + 1);
}

// std::vector<coord> blackhole_coords;  // the resting destination for the
// aliens

// static inline std::stack<Aliens*> alien_civilisation;   // pushed to this,
// once they reach the blackhole

// std::tuple < Equation, Equation, Equation > position_equations; // these
// three equations describe the position of the alien

// int current_var;    // current variable used on which the position was
// evaluated, can be used by another entity to calculate where this alien is....
// it won't share the current position with others directly... it's an alien, we
// have to find it :D

void Aliens::simulateExistence() {
    this->should_travel = true;

    this->current_var = 0; // initialise variable with 0

    coord current_coord;

    current_coord.mX = std::get<0>(position_equations).eval(this->current_var);
    current_coord.mY = std::get<1>(position_equations).eval(this->current_var);
    current_coord.mZ = std::get<2>(position_equations).eval(this->current_var);

    // it changes location throughout it's simulation, until it finds it's
    // civilisation
    while (should_travel) {
        LOGGER::log_msg("Aliens #{} ported to #{}", this->_id, current_coord);

        for (const auto &blackhole : Aliens::blackhole_coords) {
            if (current_coord == blackhole) {
                alien_civilisation.push(this); // join the civilisation :D
                LOGGER::log_msg("Aliens #{} reached the civilisation... All "
                                "the best, Bye Alien '_' ",
                                this->_id);

                break; // break through this cycle of travelling, reach a
                       // blackhole
            }
        }

        this->current_var = std::rand() % 5;

        current_coord.mX =
            std::get<0>(position_equations).eval(this->current_var);
        current_coord.mY =
            std::get<1>(position_equations).eval(this->current_var);
        current_coord.mZ =
            std::get<2>(position_equations).eval(this->current_var);

        // 0.8 unit time
        std::this_thread::sleep_for(statics::UNIT_TIME * 0.8);
    }
}

void Aliens::pauseExistence() {
    should_travel = false; // no synchronisation for now
}

Aliens::Aliens(World *)
    : Entity(Entity_Types::CUSTOM),
      position_equations("x^2 - 3x + 1", "x^2 - 3x + 1", "x^2 - 3x + 1") {
    Aliens::blackhole_coords = {{0, 0, 0}, {5, 10, 5}};
    LOGGER::log_msg("Aliens... aa gaye :D");
}

std::optional<Entity_Point> Aliens::getPrimaryPos() const {
    return std::nullopt;
    // TODO: Compute the GraphBox, and return a EntityPoint
    // return {
    //     std::get<0>(position_equations).eval(this->current_var),
    //     std::get<1>(position_equations).eval(this->current_var),
    //     std::get<2>(position_equations).eval(this->current_var)
    // };
}

EntityState *Aliens::_get_current_state() const {
    return new EntityState(Entity_Types::CUSTOM);
}
