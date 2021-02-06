#pragma once

#include <vector>
// #include <queue>
#include <thread>
#include <future>
#include <stack>

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
	Verse const* parent_verse;
	World_Node_Ptr root;
	int16_t num_nodes{0};

 	struct {
	// private:
	 	World_Node_Ptr __latest_world_node;  //pointer to the latest world currently running

	// public:
	// 	std::mutex tree_mutex;
	// 	const World_Ptr get_latest_world();
	// 	void update_node_time();
	} _fast_access_data;    // temporary data for fast access, to currently running world

public:
	const std::shared_ptr<Display> access_disp_manager() const;

	bool initTree(std::promise<void>& creation_promise);
	void destructTree();

	World_Node_Ptr const getRootNode() const {
		return this->root;
	}

	friend class Display;

	World_Tree(Verse*, std::shared_ptr<Display> displayManager);
	~World_Tree();
};
