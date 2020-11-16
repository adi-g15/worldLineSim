#pragma once

#include "forward_decl.hpp"	// for World_Tree
#include "id_creator.hpp"

#include <map>
#include <memory>
#include <thread>
#include <utility>
#include <future>

	// The world tree will mostly work almost same as array
class Verse{
	typedef World* World_Ptr;
public:
	std::promise<bool> big_bang();    // brambha, the creator
	void render_screen();    // vishnu, the carer
	std::promise<bool> kaal_day(std::string_view origin); // mahesh, the destroyer
	 // @don't try to destruct the origin, since that will just be kind of a deadlock like situation, both trying to stop the other

	std::shared_ptr<Display> displayManager; // this class will manage the rendering
	std::map< id_type, World_Node* > mapping_of_world_id_to_world_node_ptr; // to call pause on a particular world `through` the node

	Verse();
	~Verse();
private:
	// std::unordered_map< State, World* > stateMap;   //If a particular state is already on the tree, resume that world instead of creating a totally new node, which will effectively just be an array
	std::unique_ptr<World_Tree> worldTree;    //the tree will also hold number of nodes, and other properties

	friend class Display;
};

/**
 * @notes @me ->
 * 
 * 15/11/20 - Instead of `World_Tree worldTree`, converted it to a pointer (unique_ptr) to solve and be able to use it despite of `Incomplete Declaration` (by now you know very well i knew ways to solve this, i preferred this, since i don't want to end with .cpp files even for small .hpp files (maybe in future that will still be done))
*/