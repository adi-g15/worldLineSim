#pragma once

#include "forward_decl.hpp"
#include "declarations.hpp"
#include "entity.hpp"
#include "graphMat/direction.hpp"
#include "graphMat/graph_box.hpp"

#include <vector>
#include <memory>
#include <cstdint>

// @note - `directionalPath` is alias for a `std::vector`, while `SnakeBody` is an alias for a `std::list`

class Snake: public Entity{
    // typedef std::shared_ptr<World> World_Ptr;
    typedef World* World_Ptr;
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
    const _coord<dimen_t>& getPos() const override;

    void simulateExistence() override;  // calls moveForward, and other logic, for it to exist `independently (other than the needed interactions b/w entities)` from other entities

    const _coord<dimen_t>& getHeadCoord() const;
    int getUniqProp() const override;

    const Graph_Box<_box>* getHead() const;

    Snake(const World_Ptr);
    Snake(const World_Ptr, uint16_t);
private:

    // std::vector< coord_type > body;
    SnakeBody body;
    const Graph_Box<_box>* head;    // just a constant pointer to the actual box in graph

    bool eatFood(); //returns true if it can eat, else false and no change
    bool moveForward(); // continue on the acquired path, or if it's not available, find a new one
};
