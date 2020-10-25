#pragma once

#include <vector>

enum Direction: uint8_t{ // intentionally not enum class now
    UP,
    DOWN,
    RIGHT,
    LEFT
};

// could use std::queue or std::list too, that would allow popping too, but i have not done so that the allocations are at minimum
typedef std::vector<Direction>  directionalPath;
