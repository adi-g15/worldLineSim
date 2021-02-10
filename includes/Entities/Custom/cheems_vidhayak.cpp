#include "Entities/cheems_vidhayak.hpp"

#include "world.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <graphMat/iterators.hpp>

void VidhayakJi::simulateExistence()
{
    this->is_jinda = true;
    std::size_t i = 0;
    //Graph_Box_3D<Box>* rand_box = this->parent_world->world_plot[this->parent_world->world_plot.getRandomCoord()];
    Graph_Box_3D<Box>* rand_box = this->parent_world->world_plot[{0, 0, 0}];

    while (this->is_jinda)
    {
        const Direction curr_dir = statics::directions[i % statics::directions.size()];

        if (rand_box == nullptr)   break;
        std::cout << "Vidhayak Ji " << this->_id << " Rabin ko dhundh rahe hai... [" << this->curr_pos << "]" << std::endl;

        graphMat::LinearIterator<Box> linear_iter(rand_box, curr_dir);

        graphMat::DirectionalIterator<Box, Direction::URDHWA> z_iter(rand_box);
        graphMat::DirectionalIterator<Box, Direction::UTTAR> y_iter(rand_box);
        graphMat::DirectionalIterator<Box, Direction::PASHCHIM> x_iter(rand_box);

        int counter = std::rand() % 5;
        for (;linear_iter && counter != 0; ++linear_iter) {
            --counter;
        }
        
        bool flag = false;
        ++z_iter;
        ++y_iter;
        ++x_iter;
        {
            graphMat::NeighbourIterator<Box> neighbour_iter(z_iter.curr_box);
            while (neighbour_iter)
            {
                if (neighbour_iter->getData().hasEntities()) {
                    std::clog << "Bakra mil gaya hai... Vidhayak moving" << std::endl;
                    flag = true;
                }
                ++neighbour_iter;
            }
        }
        {
            graphMat::NeighbourIterator<Box> neighbour_iter(y_iter.curr_box);
            while (neighbour_iter)
            {
                if (neighbour_iter->getData().hasEntities()) {
                    std::clog << "Bakra mil gaya hai... Vidhayak moving" << std::endl;
                    flag = true;
                }
                ++neighbour_iter;
            }
        }
        {
            graphMat::NeighbourIterator<Box> neighbour_iter(x_iter.curr_box);
            while (neighbour_iter)
            {
                if (neighbour_iter->getData().hasEntities()) {
                    std::clog << "Bakra mil gaya hai... Vidhayak moving" << std::endl;
                    flag = true;
                }
                ++neighbour_iter;
            }
        }
        
        rand_box = linear_iter.curr_box;
        this->curr_pos = rand_box != nullptr ? rand_box->get_coordinate() : coord{0, 0, 0};

        if (!flag) std::clog << "Vidhayak ji khali haath" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(statics::UNIT_TIME * 1000)));
    }

    std::clog << "VidhayakJi ka career khatm... ab koi aur dal me judenge :-)" <<  this->_id << std::endl;
    //LOGGER::log_it_verb(6, "VidhayakJi ka career khatm... ab koi aur dal me judenge :-)", this->_id);
}

EntityState VidhayakJi::_get_current_state() const
{
    return VidhayakState(this);
}

VidhayakJi::VidhayakJi(World* parent_world) : Entity(Entity_Types::HUMAN)
{
    this->parent_world = parent_world;
    std::cout << "VidhayakJi #" << this->_id << " jinda ho chuke hai !!" << std::endl;
}
