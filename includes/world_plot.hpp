#pragma once

#include "graphMat/graph__square_mat_decl.hpp"
#include "includes/declarations.hpp"

/*MUST ALWAYS BE ON A SINGLE THREAD AT MAX, SINCE TEMPORARY VARIABLES ARE USED AS MEMBER OF THIS CLASS*/

// @future - If it lives on its own thread, put a auto expansion logic, that sleeps for 1 time unit, and then calls expand()
class WorldPlot: public Square_Matrix<_box>{
    const World* parent_world;

    public:
        bool isPathClear( const Graph_Box<_box>* origin, const directionalPath& path ) const;
        directionalPath getShortestPathToFood( const Graph_Box<_box>* origin ) const;    // @future - For optimising purpose, use the food coords in parent_world (eg. to go search the direction which has the food, for eg, if it is in a coord on right, only iterate through those)

        void expand();   //expands this->__temp.expansion_speed unit on each side
        void __expand_n_units();    //to be used when there's rate

        struct {
            float expansion_speed{ statics::init_expansion_speed }; //initially it will expand statics::init_expansion_speed unit at time, each side
            float free_space_ratio{100f};

            _timePoint time_since_speed_updated{0}; //after 10 time units, the __temp.expansion_speed will be decremented/reset, so as to not unecessary keep increasing storage
        } __temp;   //temporary vars
        

    WorldPlot(const World*);
    WorldPlot(const WorldPlot&) = delete;
    WorldPlot(const WorldPlot&&) = delete;

};
