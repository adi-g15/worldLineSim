#include "verse.hpp"
#include "world_tree.hpp"
#include "display/display.hpp"
#include "graphMat/graph__square_mat.hpp"

#include <iostream>	// @debug
#include <memory>
#include <stdexcept>
#include <thread>
#include <future>

	// @note - it should be more like, returning a promise, rather than a future... only logically it should hold that the receiver can poll this object, to know whether the task is completed
std::promise<bool> Verse::big_bang(){
   // init_tree should be ASYNCHRONOUS, and also starts, and manages the simulation itself
	std::promise<bool> creation_promise;

		 // creation of the first world, to ever exist in the particular verse
	this->worldTree->initTree(creation_promise);	// WorldTree::init() will create the first node

	this->render_screen();

	creation_promise.set_value(true);   // @DEBUG @note - LET IT REMAIN FOR NOW, until WorldTree::init_tree, has the capability of handling this promise implemented
	return creation_promise; // @future use a std::condition_variable to return in initTree
}

// @note - Likely, we won't be needing this std::promise being returned from kaal_day, since the destruction likely occurs in the destructors, and for setting value in destructors, we will need to have the pointer to the promise to set_value, since we can't pass anything to kaal_day
std::promise<bool> Verse::kaal_day(std::string_view origin){
	std::promise<bool> destruction_promise;
	// @todo - Cause the destruction

	destruction_promise.set_value(true);
	return destruction_promise;
}

void Verse::render_screen(){
	// this->displayManager->render();	// @note - Or may use disp::printScreen() too, and let the node_adapters trigger UI updates themselves from different threads

	this->displayManager->printScreen();
	// this->displayManager->newNodeAdapter(nullptr);

	// std::cout << "Back here" << std::endl;	// @debug
	// Display::get_async_input().wait();	// @debug

	// displayManager->main_area->refresh();
	// std::cout << "Back Again" << std::endl;	// @debug
	// Display::get_async_input().wait();	// @debug

	// wrefresh(stdscr);
	// std::cout << "Back Again2" << std::endl;	// @debug
	// Display::get_async_input().wait();	// @debug

}

Verse::Verse(): displayManager(new Display(this)), worldTree(new World_Tree(this->displayManager)){

}

Verse::~Verse(){
	this->displayManager->showExit();	// show Exit message on the screen, till the displayManger is automatically gets destructed after its destrcutor is called, and that is when everything is removed

	this->kaal_day("Shiv").get_future().wait();	// source = "Shiv" means the destructor is the 'origin' (see the declaration of kaal_day)

	// LOGGER << "Verse has been destructed"
}

	// @note @deprecated - No more control here, too, now World_Tree will do this (and in fact, all world_nodes 'automatically' start the simulation of the world they hold)
// void Verse::runSimulation(){
//     // @todo

//     while( true ){
//         this->currWorld->runNextUnitTime();
//         std::this_thread::sleep_for(std::chrono::microseconds((int) statics::UNIT_TIME * 1000));   //wait for 1 second before executing the nextUnitTime of world
//     }
// }

	// @note @deprecated - (WAS NEVER COMPLETE)
// bool Verse::createWorld(_timePoint t){
//     return this->createWorld(this->currWorld, t);
// }

// bool Verse::createWorld(World_Ptr world, _timePoint){
//     if( world == nullptr )    throw std::logic_error("createWorld called before initialising the worldTree");
//     world->stopSimulation();

//     World_Ptr newWorld = std::make_shared<World>(*(world.get()));   // CAUTION: dereferencing shared_ptr::get()
//     this->worldTree.insert(newWorld);
//     this->currWorld = newWorld;

// }
