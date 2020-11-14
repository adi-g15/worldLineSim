#pragma once

#include "graphMat/graph__square_mat_decl.hpp"
#include "includes/declarations.hpp"

/*MUST ALWAYS BE ON A SINGLE THREAD AT MAX, SINCE TEMPORARY VARIABLES ARE USED AS MEMBER OF THIS CLASS*/

// @future - If it lives on its own thread, put a auto expansion logic, that sleeps for 1 time unit, and then calls auto_expand()
class WorldPlot: public Square_Matrix<_box>{
    typedef int32_t dimen_t;
    typedef World* World_Ptr;

    const World_Ptr parent_world;
    Graph_Box<_box>* food;   // @future - food may have different points/nutitional values too

    void auto_expand();   //expands this->__temp.expansion_speed unit on each side
    void __expand_n_units(int8_t n);    //to be used when there's rate
    const Graph_Box<_box>* get_box(const _coord<dimen_t>& position) const{
        // @todo - Return the graph_box with that coordinate

        return &(this->origin);    // @debug - just for now
    }

    public:
        const Graph_Box<_box>* get_food() const{
            return this->food;
        }
        void createFood();
        dimen_t getCurrentOrder() const;
        bool isPathClear( const Graph_Box<_box>* origin, const directionalPath& path ) const;
        directionalPath getShortestPathToFood( const Graph_Box<_box>* origin ) const;    // @future - For optimising purpose, use the food coords in parent_world (eg. to go search the direction which has the food, for eg, if it is in a coord on right, only iterate through those)
        void getShortestPathToFood( const Graph_Box<_box>* origin, directionalPath& ) const;

        void start_auto_expansion();

        struct {
            float expansion_speed{ statics::init_expansion_speed }; //initially it will auto_expand statics::init_expansion_speed unit at time, each side
            float free_space_ratio{100.0f};

            _timePoint time_since_speed_updated{0}; //after 10 time units, the __temp.expansion_speed will be decremented/reset, so as to not unecessary keep increasing storage
        } __temp;   //temporary vars


    WorldPlot(const World_Ptr);

    friend class World;

};
