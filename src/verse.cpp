#include "verse.hpp"
#include "display/display.hpp"
#include "graphMat/cube_mat.hpp"
#include "world_tree.hpp"

import <future>;
import <iostream>; // @debug
import <memory>;
import <stdexcept>;
import <thread>;

// Update: Feb 4 -> It no longer returns a promise or a future, display takes
// over the main thread, and BLOCKS
// @note - it should be more like, returning a promise, rather than a future...
// only logically it should hold that the receiver can poll this object, to know
// whether the task is completed
void Verse::big_bang() {
    this->multiverse_forest.back().initTree(
        /*this->creation_promise*/); // WorldTree::init() will create the first
                                     // node

    this->displayManager->startDisplay();
}

void Verse::add_world_tree() {
    this->multiverse_forest.emplace_back(this, this->displayManager);
    this->multiverse_forest.back().initTree();
}

// @note - Blocks until everything is stopped
void Verse::kaal_day(std::string_view origin) {
    if (origin != "Shiv") {
        this->displayManager
            ->showExiting(); // show Exit message on the screen, till the
                             // displayManger is automatically gets destructed
                             // after its destrcutor is called, and that is when
                             // everything is removed
    }

    for (auto &worldTree : multiverse_forest) {
        worldTree.destructTree();
    }

    if (origin != "Shiv") { // if not destructor then show these messages
        this->displayManager->stopDisplay();
    }
}

Verse::Verse() : displayManager(new Display(this)), _ID() {
    multiverse_forest.emplace_back(
        this, this->displayManager); // one tree will already be in the forest
    // worldTree.reset(new World_Tree(this, this->displayManager));
}

Verse::~Verse() {
    this->kaal_day("Shiv"); // source = "Shiv" means the destructor is the
                            // 'origin' (see the declaration of kaal_day)

    // LOGGER << "Verse has been destructed"
}

// @note @deprecated - No more control here, too, now World_Tree will do this
// (and in fact, all world_nodes 'automatically' start the simulation of the
// world they hold)
// void Verse::runSimulation(){
//     // @todo

//     while( true ){
//         this->currWorld->runNextUnitTime();
//         std::this_thread::sleep_for(statics::UNIT_TIME));   //wait before
//         executing the nextUnitTime of world
//     }
// }

// @note @deprecated - (WAS NEVER COMPLETE)
// bool Verse::createWorld(_timePoint t){
//     return this->createWorld(this->currWorld, t);
// }

// bool Verse::createWorld(World_Ptr world, _timePoint){
//     if( world == nullptr )    throw std::logic_error("createWorld called
//     before initialising the worldTree"); world->stopSimulation();

//     World_Ptr newWorld = std::make_shared<World>(*(world.get()));   //
//     CAUTION: dereferencing shared_ptr::get()
//     this->worldTree.insert(newWorld);
//     this->currWorld = newWorld;

// }
