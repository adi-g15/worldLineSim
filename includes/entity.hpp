#pragma once

#include <string>
#include <string_view>

class Entity{   // Entity class for now won't hold any connection with a particular world
private:
    const uint16_t _ENTITY_ID_;   //some random ID
    std::string type;
public:
    Entity(std::string_view s): type(s), _ENTITY_ID_(std::hash(s)*random()){}
};
