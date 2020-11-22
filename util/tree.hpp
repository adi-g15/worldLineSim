#pragma once

#include <map>
#include <list>
#include <memory>

template<typename data_type>
struct _tree_node{
	typedef _tree_node<data_type> tree_node;
	typedef tree_node* tree_node_ptr;

private:
//	std::list<tree_node_ptr> nodes;
	data_type data;
public:

	_tree_node(data_type&& data): data(data){}
};


template< typename node_dtype >
class Tree{
	node_dtype* root;    // could have been unique_ptr, but we need a pointer in World for the `current active world`
	std::list< node_dtype > nodes;   // we can't have std::list of raw references (though using wrapper should work, but not needed here)

	public:
		Tree(): root(nullptr){}
		Tree(node_dtype& root): root(root){
			this->root = root;
		}

		node_dtype& find();  //@todo - should take a lambda
		void insert(node_dtype&);
		size_t size();
};
