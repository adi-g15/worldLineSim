#pragma once

#include "forward_decl.hpp"
#include "declarations.hpp"
#include "entity.hpp"
#include "graphMat/direction.hpp"
#include "graphMat/graph_box.hpp"

#include <vector>
#include <memory>
#include <cstdint>


//Later instead of a path being a vector of coords, have it as a vector of directioins, and also store the direction of this snake
class Snake: public Entity{
    // typedef std::shared_ptr<World> World_Ptr;
    typedef World* World_Ptr;
    typedef _coord<int32_t> coord_type;
    typedef int32_t dimen_t;
    typedef std::make_unsigned_t<dimen_t> udimen_t;

    typedef std::list<Direction> SnakeBody; //list because, popping at back and pushing in front both required quite regulRLY, SO CANT use vector, shift is O(N)
public:
    const World_Ptr parent_world;

    // std::vector< coord_type > curr_path;
    directionalPath curr_Path;
    udimen_t length;   //starts with _world.init_length

    void _Action1() override;   //calls moveForward()
    void _Action2() override;   //calls eatFood()

    const coord_type& getHeadCoord() const;

    Graph_Box<_box>* getHead();
    const Graph_Box<_box>* getHead() const;

    Snake(const World_Ptr);
    Snake(const World_Ptr, int init_len);
private:
    // std::vector< coord_type > body;
    std::vector< SnakeBody > body;
    Graph_Box<_box>* head;

    bool eatFood(); //returns true if it can eat, else false and no change
    bool moveForward(); // continue on the acquired path, or if it's not available, find a new one
};
