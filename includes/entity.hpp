#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "random.hpp"

typedef void (*Action_Ptr)(void);

class Entity: _ID{   // Entity class for now won't hold any connection with a particular world
private:
    std::string type;
    std::vector<Action_Ptr> supported_Operations;
    // std::vector<void (*)()> supported_Operations;
    // std::vector<void()> supported_Operations;   //same as above
public:
    Entity(std::string_view s): 
        _ID(), 
        type(s)
    {
        // supported_Operations.push_back( &foo );  //works, void(void) in global scope
        // supported_Operations.push_back( &Entity::_Action1 ); //doesn't work
        // supported_Operations.push_back( &Entity::_Action1 ); //doesn't work
        // supported_Operations = {&Entity::_Action1, &Entity::_Action2};
    }
    auto getEntityId(){ return this->_id; }

    virtual void _Action1();    //only 2 actions supported as of now
    virtual void _Action2();
};
