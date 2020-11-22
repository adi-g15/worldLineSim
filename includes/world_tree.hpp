#pragma once

#include <vector>
// #include <queue>
#include <thread>
#include <future>
#include <stack>

#include "util/tree.hpp"
#include "forward_decl.hpp"

/*
NOTE - This time, i will first complete World_Tree class from start, and then what seems to be common to other trees, i will slowly shift it to the tree class, and then all those properties are to inhgherited in the Wrd_tree class from Tree class
*/
// it will actuall ystore dynamiucally allocated objects, not just there pointers
// template<typename node_data_type = World>  // @for_now - For now only, we are giving it this default templated version
class World_Tree{
	// typedef Tree_Node* Tree_Node_Ptr;
	typedef World_Node* World_Node_Ptr;
	// typedef _tree_node<World>* World_Node_Ptr;
private:
	std::shared_ptr<Display> displayManager;    // the displayManager for the verse
	const Verse* parent_verse;
	World_Node_Ptr root;
	// std::vector<Tree_Node_Ptr> all_nodes;    //vector of pointers to all nodes, this maybe additional storage, but gives ease destructing, and to know number of nodes
	int16_t num_nodes;

, 	struct {
	// private:
	// 	World_Node_Ptr __latest_world_node;  //pointer to the latest world currently running

	// public:
	// 	std::mutex tree_mutex;
	// 	const World_Ptr get_latest_world();
	// 	void update_node_time();
	} _fast_access_data;    // temporary data for fast access, to currently running world

public:
	const std::shared_ptr<Display> access_disp_manager() const;

	bool initTree(std::promise<bool>& creation_promise);

	World_Tree(std::shared_ptr<Display> displayManager);
	~World_Tree();
};
