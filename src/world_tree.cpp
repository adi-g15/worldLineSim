#include "world_tree.hpp"
#include "verse.hpp"
#include "world.hpp"
#include "world_node.hpp"

/*
NOTE - This time, i will first complete World_Tree class from start, and then
what seems to be common to other trees, i will slowly shift it to the tree
class, and then all those properties are to inhgherited in the Wrd_tree class
from Tree class
*/
// it will actuall ystore dynamiucally allocated objects, not just there
// pointers template<typename node_data_type = World>  // @for_now - For now
// only, we are giving it this default templated version
const Display *World_Tree::access_disp_manager() const {
    return this->parent_verse->disp_manager();
}

Display *World_Tree::access_disp_manager() {
    return this->parent_verse->disp_manager();
}

bool World_Tree::initTree() {
    if (this->root != nullptr)
        return true; // already initialised

    this->root = new World_Node(this);
    this->num_nodes = 1;

    return true; // @debug
}

// @note - Blocks until the tree is destructed
void World_Tree::destructTree() {
    World_Node_Ptr temp = root;

    // while (temp->diverged_child.empty())
    //{
    //    temp = temp->diverged_child.front();
    //}
    // while (temp != root)
    //{
    //    temp = temp->parent_node;

    //    delete temp->diverged_child;
    //}
}

World_Tree::World_Tree(Verse *parent_verse, Display_Ptr displayManager)
    : parent_verse(parent_verse), root(nullptr), num_nodes(0) {
    if (!displayManager) {
        throw std::logic_error("Expected a display Manager, that is incharge "
                               "of the display of the verse");
    }

    this->displayManager = displayManager;
}

World_Tree::~World_Tree() { this->destructTree(); }
