#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <any>

#include "id_creator.hpp"

typedef void (*Action_Ptr)(void);

struct Prop{    // @future - See the @future comment on the Entity::getUniqProp() function
    std::any data;
};

struct SnakeProp: Prop{
    // int data;   // for snake, the unique property (ie. this `data`) will be an int, ie. length
};

template<typename dimen_t> // @assert - dimen_t must be integral type
struct _coord;  // forward declaration

enum class entity_Types: uint8_t {
    SNAKE,
    // more can be added later, it's made to be expandible, though do see the member declarations as for what ANY entity must have
};

// In our case, each entity will have atleast one id, and can have 2 ids, if their simulatoon is running (the second ID being the thread::id they are running on)

// Meant to be inherited from; Also since some member functions are pure virtuals, so you can't have an object of Entity either
class Entity: public _ID{
    typedef int32_t dimen_t;
protected:
    entity_Types type;
    std::vector<Action_Ptr> supported_Operations;
    // std::vector<void (*)()> supported_Operations;
    // std::vector<void()> supported_Operations;   //same as above
public:
    id_type getEntityId() const{
        return this->_id;
    }
    virtual const _coord<dimen_t>& getPos() const = 0;
    virtual void _Action1() = 0;    //only 2 actions supported as of now
    virtual void _Action2() = 0;

    // @future -> Since the return type of this is fixed to int, it is restricted currently, but to allow more flexibility, the method return type can be changed to return an object of 'Prop', which is just a wrapper over the data that will be returned (or simply change return type to std::any)
    virtual int getUniqProp() const = 0;  // each entity will have a unique property, for eg. snake's unique property is its length

    virtual void simulateExistence() = 0;   // simulate the work when on a single thread

    Entity(entity_Types type):
        _ID(),
        type(type)
    {
        // supported_Operations.push_back( &foo );  //works, void(void) in global scope
        // supported_Operations.push_back( &Entity::_Action1 ); //doesn't work
        // supported_Operations.push_back( &Entity::_Action1 ); //doesn't work
        // supported_Operations = {&Entity::_Action1, &Entity::_Action2};
    }

    friend class World;
};
