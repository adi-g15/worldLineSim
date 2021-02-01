#include "verse.hpp"
#include "world_tree.hpp"
#include "display/display.hpp"
#include "graphMat/cube_mat.hpp"

#include <iostream>	// @debug
#include <memory>
#include <stdexcept>
#include <thread>
#include <future>

	// @note - it should be more like, returning a promise, rather than a future... only logically it should hold that the receiver can poll this object, to know whether the task is completed
std::future<void> Verse::big_bang(){
   // init_tree should be ASYNCHRONOUS, and also starts, and manages the simulation itself
		 // creation of the first world, to ever exist in the particular verse
	this->worldTree->initTree(this->creation_promise);	// WorldTree::init() will create the first node

	this->render_screen();

	return this->creation_promise.get_future(); // @dropped_idea - use a std::condition_variable to return in initTree
}

// @note - Blocks until everything is stopped
void Verse::kaal_day(std::string_view origin){
	this->worldTree->destructTree();

	// @future @log - Stop the logger here too
	// this->logger.stop();
}

void Verse::render_screen(){
	// this->displayManager->render();	// @note - Or may use disp::printScreen() too, and let the node_adapters trigger UI updates themselves from different threads

	// earlier this was active //this->displayManager->printScreen();
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

Verse::Verse(): displayManager(new Display(this)) {
	worldTree.reset(new World_Tree(this, this->displayManager));
}

Verse::~Verse(){
	this->displayManager->showExit();	// show Exit message on the screen, till the displayManger is automatically gets destructed after its destrcutor is called, and that is when everything is removed

	this->kaal_day("Shiv");	// source = "Shiv" means the destructor is the 'origin' (see the declaration of kaal_day)

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
