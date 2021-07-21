#include "Entities/Custom/mathematicians.hpp"

#include <thread>
#include <chrono>

void Mathematician::simulateExistence() {
    this->should_research = true;

    // it changes location throughout it's simulation, until it finds it's civilisation
    while ( should_research ) {
        coord analyzed_location( std::rand() % 40, std::rand() % 40, std::rand() % 40 );
        shared_knowledge.push( analyzed_location );    // join the civilisation :D

        if ( std::rand() % 4 == 1 ) {
            LOGGER::log_msg("Mathematician {} [#{}] has analyzed the coordinate: #{}...", this->name, this->_id, analyzed_location);
        } else if ( std::rand() % 2 == 1 ) {
            LOGGER::log_msg("{} [Mathematician] embracing the sky's beauty", this->name);
        } else {
            LOGGER::log_msg("{} [Mathematician] thinking", this->name);
        }

        // 2 second sleep
        std::this_thread::sleep_for( std::chrono::milliseconds( static_cast<int>(statics::UNIT_TIME * 2000) ));
    }
}

void Mathematician::pauseExistence() {
    should_research = false;    // no synchronisation for now
}

std::optional<Entity_Point> Mathematician::getPrimaryPos() const {
    return std::nullopt;
}

Mathematician::Mathematician(World*): Entity(Entity_Types::CUSTOM) {
    LOGGER::log_msg("[CELEBRATION] The verse... got a new mathematician :D");

    name = "Mishra";
}

EntityState* Mathematician::_get_current_state() const {
    return new EntityState(Entity_Types::CUSTOM);
}
