#pragma once

#include <map>
#include <list>
#include <memory>

template<typename data_type>
struct _tree_node{
public:
    typedef _tree_node<data_type> tree_node;
    typedef tree_node* tree_node_ptr;
    data_type data;

    std::list<tree_node_ptr> nodes;
    _tree_node(data_type& data): data(data){}
};


template< typename node_type >
class Tree{
    node_type* root;    // could have been unique_ptr, but we need a pointer in World for the `current active world`
    std::list< node_type > nodes;   // we can't have std::list of raw references (though using wrapper should work, but not needed here)

    public:
        Tree(): root(nullptr){}
        Tree(node_type& root): root(root){
            this->root = root;
        }

        node_type& find();  //@todo - should take a lambda
        void insert(node_type&);
        size_t size();
};
