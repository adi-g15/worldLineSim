#pragma once

#include "world.hpp"
#include "util/tree.hpp"
#include "declarations.hpp"
#include <memory>

    // The world tree will mostly work almost same as array
class Verse{
    typedef std::shared_ptr<World> World_Ptr;
public:
    void big_bang();

    Verse();
private:
        //whenever a new world is to be created, this will be used
    // std::unordered_map< State, World* > stateMap;   //If a particular state is already on the tree, resume that world instead of creating a totally new node, which will effectively just be an array
    Tree< World_Ptr > worldTree;    //the tree will also hold number of nodes, and other properties
    World_Ptr currWorld;

    void runSimulation();
    bool createWorld(_timePoint);   //forks the new world from current World
    bool createWorld(World_Ptr, _timePoint);

};