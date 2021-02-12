#include "Entities/rabin.hpp"

#include "world.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <graphMat/iterators.hpp>

void Rabin::simulateExistence()
{
    this->is_jinda = true;
    std::size_t i = 0;
    Graph_Box_3D<Box>* rand_box = this->parent_world->world_plot[ this->parent_world->world_plot.getRandomCoord() ];

    while (this->is_jinda)
    {
        const Direction curr_dir = statics::directions[i % statics::directions.size()];

        if (rand_box == nullptr)   break;
        graphMat::LinearIterator<Box> linear_iter(rand_box, curr_dir);

        if (rand_box->get_coordinate().mX % 2 == 0) LOGGER::log_msg("Rabin marne se bach gaya");
        else LOGGER::log_msg("Rabin marwa raha hai...");

        int counter = std::rand() % 5;
        for (;linear_iter && counter != 0; ++linear_iter) {
            --counter;
        }

        rand_box = linear_iter.curr_box;

        std::this_thread::sleep_for( std::chrono::milliseconds( static_cast<int>(statics::UNIT_TIME * 1000) ));
    }

    LOGGER::log_msg("Rabin #{} ruk gaya", this->_id);
}

Rabin::Rabin(World* parent_world): Entity(Entity_Types::HUMAN)
{
    this->parent_world = parent_world;
    LOGGER::log_msg("Rabin jinda ho gaya !!");
}
