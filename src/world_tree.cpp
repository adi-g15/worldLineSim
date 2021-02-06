#include "world_tree.hpp"
#include "world.hpp"
#include "world_node.hpp"
#include "verse.hpp"

/*
NOTE - This time, i will first complete World_Tree class from start, and then what seems to be common to other trees, i will slowly shift it to the tree class, and then all those properties are to inhgherited in the Wrd_tree class from Tree class
*/
// it will actuall ystore dynamiucally allocated objects, not just there pointers
// template<typename node_data_type = World>  // @for_now - For now only, we are giving it this default templated version
const std::shared_ptr<Display> World_Tree::access_disp_manager() const{
    return this->parent_verse->disp_manager();
}

bool World_Tree::initTree(std::promise<void>& creation_promise){
    if (this->root != nullptr)   return true;   // already initialised

    this->root = new World_Node(this, nullptr, statics::BIG_BANG_TIME);
    this->num_nodes = 1;
    this->_fast_access_data.__latest_world_node = this->root;

    creation_promise.set_value();
    return true;    // @debug
}

// @note - Blocks until the tree is destructed
void World_Tree::destructTree()
{
    std::stack<World_Node_Ptr> st;
    World_Node_Ptr temp;

    if (root) st.push(root);

    while (!st.empty()) {
        temp = st.top();

        if (temp->left_node)	st.push(temp->left_node);
        if (temp->right_node)	st.push(temp->right_node);

        delete st.top();
        st.pop();
    }
}

World_Tree::World_Tree(Verse* parent_verse, std::shared_ptr<Display> displayManager) : 
    parent_verse(parent_verse),
    root(nullptr), 
    num_nodes(0)
{
    if( !displayManager ){
        throw std::logic_error("Expected a display Manager, that is incharge of the display of the verse");
    }

    this->displayManager = displayManager;
}

World_Tree::~World_Tree(){
    this->destructTree();
}
