#pragma once

#include "forward_decl.hpp"	// for World_Tree
#include "id_creator.hpp"

#include <map>
#include <memory>
#include <list>
#include <thread>
#include <utility>
#include <future>

#include "display/display.hpp"

	// The world tree will mostly work almost same as array
class Verse: public _ID{
public:
	std::promise<void> creation_promise;	// shouldn't be required now, but still letting it remain for now

	void big_bang();    // brambha, the creator

	void add_world_tree();

	void render_screen();    // vishnu, the carer
	void kaal_day(std::string_view origin); // mahesh, the destroyer
	 // @don't try to destruct the origin, since that will just be kind of a deadlock like situation, both trying to stop the other

	const Display* disp_manager() const{
		return this->displayManager;
	}
	Display* disp_manager() {
		return this->displayManager;
	}

	Verse();
	~Verse();
private:
	// std::unordered_map< State, World* > stateMap;   //If a particular state is already on the tree, resume that world instead of creating a totally new node, which will effectively just be an array
	std::list< World_Tree > multiverse_forest;    // there can be multiple independent trees if wanted so
	Display* displayManager; // this class will manage the rendering
	std::map< id_type, World_Node* > id_to_node; // to call pause on a particular world `through` the node

	friend class Display;
};

/**
 * @notes @me ->
 * 
 * 15/11/20 - Instead of `World_Tree worldTree`, converted it to a pointer (unique_ptr) to solve and be able to use it despite of `Incomplete Declaration` (by now you know very well i knew ways to solve this, i preferred this, since i don't want to end with .cpp files even for small .hpp files (maybe in future that will still be done))
*/