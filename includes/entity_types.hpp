#pragma once

#include <array>
#include <string>

    // moved here, since there will be many entities, generated, many of them being unique
enum class Entity_Types {
    SNAKE = 0,
    ROCK,
    HUMAN
    // more can be added later, it's made to be expandible, though do see the member declarations as for what ANY entity must have
};

const std::array<const std::string, 3> Entity_Type_Map = {
    "Snake",
    "Rock",
    "Human"
};
