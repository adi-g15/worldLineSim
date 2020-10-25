#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "random.hpp"

typedef void (*Action_Ptr)(void);

enum class entity_Types: uint8_t {
    SNAKE,
    // more can be added later, it's made to be expandible, though do see the member declarations as for what ANY entity must have
};

// In our case, each entity will have atleast one id, and can have 2 ids, if their simulatoon is running (the second ID being the thread::id they are running on)

// Meant to be inherited from; Also since some member functions are pure virtuals, so you can't have an object of Entity either
class Entity: _ID{
    typedef int32_t dimen_t;
protected:
    entity_Types type;
    std::vector<Action_Ptr> supported_Operations;
    // std::vector<void (*)()> supported_Operations;
    // std::vector<void()> supported_Operations;   //same as above
public:
    Entity(entity_Types type):
        _ID(), 
        type(type)
    {
        // supported_Operations.push_back( &foo );  //works, void(void) in global scope
        // supported_Operations.push_back( &Entity::_Action1 ); //doesn't work
        // supported_Operations.push_back( &Entity::_Action1 ); //doesn't work
        // supported_Operations = {&Entity::_Action1, &Entity::_Action2};
    }
    auto getEntityId(){ return this->_id; }
    virtual _coord<dimen_t> getPos() const = 0;
    virtual void _Action1() = 0;    //only 2 actions supported as of now
    virtual void _Action2() = 0;
    virtual void simulateExistence() = 0;   // simulate the work when on a single thread
};
