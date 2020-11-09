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
	std::promise<bool> big_bang();    // brambha, the creator
	void render_screen();    // vishnu, the carer
	std::promise<bool> kaal_day(std::string_view origin); // mahesh, the destroyer
	 // @don't try to destruct the origin, since that will just be kind of a deadlock like situation, both trying to stop the other

	Display displayManager; // this class will manage the rendering
	std::map< id_type, World_Node* > mapping_of_world_id_to_world_node_ptr; // to call pause on a particular world `through` the node

	Verse();
	~Verse();
private:
	// std::unordered_map< State, World* > stateMap;   //If a particular state is already on the tree, resume that world instead of creating a totally new node, which will effectively just be an array
	World_Tree worldTree;    //the tree will also hold number of nodes, and other properties

	friend class Display;
};