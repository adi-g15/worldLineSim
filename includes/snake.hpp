#pragma once

#include "forward_decl.hpp"
#include "declarations.hpp"
#include "entity.hpp"

#include <vector>
#include <memory>
#include <cstdint>

//Later instead of a path being a vector of coords, have it as a vector of directioins, and also store the direction of this snake
class Snake: public Entity{
    typedef std::shared_ptr<World> World_Ptr;
public:
    const World_Ptr parent_world;

    std::vector< _coord > curr_path;
    uint32_t length;   //starts with _world.init_length

    bool isPathClean() const;
    bool eatFood(); //returns true if it can eat, else false and no change
    bool moveForward();
    const _coord& getHead() const;

    Snake(const World_Ptr);
    Snake(const World_Ptr, uint16_t init_len);
private:
    std::vector< _coord > body;
    _coord head;
};
