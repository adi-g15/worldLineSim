#pragma once

#include <stdexcept>

class WorldCanTExpand : public std::logic_error{
    public:
        const char* what(){
            return "World cannot expand more, already at the limit !";
        }
        WorldCanTExpand(): std::logic_error(""){}
};
