#pragma once

#include <array>
#include <string>

// moved here, since there will be many entities, generated, many of them being
// unique
enum class Entity_Types {
    SNAKE = 0,
    ROCK,
    HUMAN,
    META, // NEW: currently only GDSC exists as a meta entity
    CUSTOM
    // more can be added later, it's made to be expandible, though do see the
    // member declarations as for what ANY entity must have
};

const std::array<const std::string, 5> Entity_Type_Map = {
    "Snake", "Rock", "Human", "META", "Custom"};
