#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <any>
#include <optional>

#include "forward_decl.hpp"
#include "declarations.hpp"
#include "id_creator.hpp"
#include "logger.hpp"
// #include "graphMat/graph_box.hpp"

class Bhakt: public _ID{
    using dimen_t = statics::dimen_t;

    World_Node* world_node;
protected:
    //std::vector<Action_Ptr> supported_Operations;
    // std::vector<void (*)()> supported_Operations;
    // std::vector<void()> supported_Operations;   //same as above
public:
    // Simulation functions
    virtual void simulateExistence() = 0;
    virtual void pauseExistence() = 0;

    // Time Travel functions _ That allow the time travel feature
    // NOTE: Time travel is provided by world_node which is the power his/her God has given her
    id_type worldline_id() const;

    Bhakt(World_Node* world_node):
        world_node(world_node),
        _ID()
    {
        LOGGER::log_msg("Hare krishna !!");
        // supported_Operations.push_back( &foo );  //works, void(void) in global scope
        // supported_Operations.push_back( &Entity::_Action1 ); //doesn't work
        // supported_Operations.push_back( &Entity::_Action1 ); //doesn't work
        // supported_Operations = {&Entity::_Action1, &Entity::_Action2};
    }

    // friend class World_Node;
};
