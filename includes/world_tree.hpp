#pragma once

#include "util/tree.hpp"
#include <vector>
#include <queue>
#include <thread>

/*
NOTE - This time, i will first complete World_Tree class from start, and then what seems to be common to other trees, i will slowly shift it to the tree class, and then all those properties are to inhgherited in the Wrd_tree class from Tree class
*/
// it will actuall ystore dynamiucally allocated objects, not just there pointers
// template<typename node_data_type = World>  // @for_now - For now only, we are giving it this default templated version
class World_Tree : public Tree<World>{
	// typedef Tree_Node* Tree_Node_Ptr;
	typedef World_Node* World_Node_Ptr;
	// typedef _tree_node<World>* World_Node_Ptr;
private:
	Display* displayManager;    // the displayManager for the verse
	World_Node_Ptr root;
	// std::vector<Tree_Node_Ptr> all_nodes;    //vector of pointers to all nodes, this maybe additional storage, but gives ease destructing, and to know number of nodes
	int16_t num_nodes;

	struct {
	private:
		World_Node_Ptr __latest_world_node;  //pointer to the latest world currently running

	public:
		std::mutex tree_mutex;
		const World_Ptr get_latest_world(){
			std::scoped_lock s(tree_mutex);
			return __latest_world_node->get_world();
		}
		void update_node_time(){    // @note - Call this when a world is going to be stopped
			__latest_world_node->paused_time = __latest_world_node->get_world()->currentTime;
		}
	} _fast_access_data;    // temporary data for fast access, to currently running world

public:
	bool initTree(World_Node_Ptr root_node){    //should be called after Verse::big_bang(), to initiate a world, and set it as the root node


	}

	World_Tree(Display* displayManager) : root(nullptr), num_nodes(0){
		if( !displayManager ){
			throw std::logic_error("Expected a display Manager, that is incharge of the display of the verse");
		}

		this->displayManager = displayManager;
	}
	// @tip - Use delegation to have a `master` constructor that all other will call, or if not a single master, then decrease the duplicacy in the constructor body
	World_Tree(World& root_data, Display* displayManager) : root(new World_Node(root_data)), num_nodes(1){
		if( !displayManager ){
			throw std::logic_error("Expected a display Manager, that is incharge of the display of the verse");
		}

		this->displayManager = displayManager;
	}
	World_Tree(World&& root_data, Display* displayManager) : root(new World_Node(root_data)), num_nodes(1){}
	~World_Tree(){
		if( !displayManager ){
			throw std::logic_error("Expected a display Manager, that is incharge of the display of the verse");
		}

		this->displayManager = displayManager;
		std::queue<World_Node_Ptr> qn;
		qn.push(root);
		World_Node_Ptr temp;

		while( !qn.empty() ){
			temp = qn.front();

			for( _tree_node<World>* i : temp->nodes ){
				qn.push(i);
			}
			delete qn.front();;
			qn.pop();
		}
	}
};
