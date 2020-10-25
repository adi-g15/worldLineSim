#pragma once

#include "world.hpp"
#include "world_tree.hpp"
#include "declarations.hpp"

#include "display.hpp"

#include <memory>
#include <thread>
#include <utility>

    // The world tree will mostly work almost same as array
class Verse{
    typedef std::shared_ptr<World> World_Ptr;
public:
    void big_bang();

    Display display_Screen; // this class will manage the rendering

    Verse();
private:
        //whenever a new world is to be created, this will be used
    // std::unordered_map< State, World* > stateMap;   //If a particular state is already on the tree, resume that world instead of creating a totally new node, which will effectively just be an array
    World_Tree worldTree;    //the tree will also hold number of nodes, and other properties

    // World_Ptr currWorld; // this to be handled by the world tree itself
    const World_Ptr get_current_world(){
        return this->worldTree.
    }


    void renderScreen();
    void runSimulation();
    bool createWorld(_timePoint);   //forks the new world from current World
    bool createWorld(World_Ptr, _timePoint);

    friend class Display;

};