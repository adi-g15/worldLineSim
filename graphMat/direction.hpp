#pragma once

#include <vector>

enum Direction: uint8_t{ // intentionally not enum class now
    UP,
    DOWN,
    RIGHT,
    LEFT
};

typedef std::vector<Direction>  directionalPath;    // can use std::queue<Dimension> too, that would allow popping too
