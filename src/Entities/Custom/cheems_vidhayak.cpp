#include "Entities/Custom/cheems_vidhayak.hpp"

#include "logger.hpp"
#include "world.hpp"
#include <chrono>
#include <graphMat/iterators.hpp>
#include <thread>

void VidhayakJi::simulateExistence() {
    this->is_jinda = true;
    std::size_t i = 0;
    // Graph_Box_3D<Box>* rand_box =
    // this->parent_world->world_plot[this->parent_world->world_plot.getRandomCoord()];
    Graph_Box_3D<Box> *rand_box = this->parent_world->world_plot[{0, 0, 0}];

    while (this->is_jinda) {
        const Direction curr_dir =
            statics::directions[i % statics::directions.size()];

        if (rand_box == nullptr)
            break;
        LOGGER::log_msg(
            "Chacha Vidhayak Ji #${} 🤝🤝 rabin ko dhundh rahe hai... [{}]",
            this->_id, this->curr_pos);

        graphMat::LinearIterator<Box> linear_iter(rand_box, curr_dir);

        graphMat::DirectionalIterator<Box, Direction::URDHWA> z_iter(rand_box);
        graphMat::DirectionalIterator<Box, Direction::UTTAR> y_iter(rand_box);
        graphMat::DirectionalIterator<Box, Direction::PASHCHIM> x_iter(
            rand_box);

        int counter = std::rand() % 5;
        for (; linear_iter && counter != 0; ++linear_iter) {
            --counter;
        }

        bool flag = false;
        ++z_iter;
        ++z_iter;
        ++y_iter;
        ++y_iter;
        ++x_iter;
        ++x_iter;
        if (z_iter) {
            graphMat::NeighbourIterator<Box> neighbour_iter(z_iter.curr_box);
            while (neighbour_iter) {
                if (neighbour_iter->getData().hasEntities()) {
                    LOGGER::log_msg(
                        "Mil gaya rabin... Vidhayak #{} action time",
                        this->_id);
                    flag = true;
                }
                ++neighbour_iter;
            }
        }
        if (y_iter) {
            graphMat::NeighbourIterator<Box> neighbour_iter(y_iter.curr_box);
            while (neighbour_iter) {
                if (neighbour_iter->getData().hasEntities()) {
                    LOGGER::log_msg(
                        "Rabin 🥲🥲... Vidhayak ${} action time",
                        this->_id);
                    flag = true;
                }
                ++neighbour_iter;
            }
        }
        if (x_iter) {
            graphMat::NeighbourIterator<Box> neighbour_iter(x_iter.curr_box);
            while (neighbour_iter) {
                if (neighbour_iter->getData().hasEntities()) {
                    LOGGER::log_msg("Mai bole to Cheems Vidhayak ${} 😎",
                                    this->_id);
                    flag = true;
                }
                ++neighbour_iter;
            }
        }

        rand_box = linear_iter.curr_box;
#ifndef GRAPH_MAT_NO_COORD
        this->curr_pos =
            rand_box != nullptr ? rand_box->get_coordinate() : coord{0, 0, 0};
#endif
        std::this_thread::sleep_for(statics::UNIT_TIME);
    }

    LOGGER::log_msg(
        "VidhayakJi #{} ka career khatm... ab koi aur dal me judenge :-)",
        this->_id);
}

VidhayakJi::VidhayakJi(World *parent_world) : Entity(Entity_Types::HUMAN) {
    this->parent_world = parent_world;
    LOGGER::log_msg("VidhayakJi #{} jinda ho chuke hai !!", this->_id);
}

EntityState *VidhayakJi::_get_current_state() const {
    return new EntityState(Entity_Types::CUSTOM);
}
