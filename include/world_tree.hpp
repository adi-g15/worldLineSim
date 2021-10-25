#pragma once

#include <vector>
// #include <queue>
#include <future>
#include <stack>
#include <thread>

#include "forward_decl.hpp"

/*
NOTE - This time, i will first complete World_Tree class from start, and then
what seems to be common to other trees, i will slowly shift it to the tree
class, and then all those properties are to inhgherited in the Wrd_tree class
from Tree class
*/
// it will actuall ystore dynamiucally allocated objects, not just there
// pointers template<typename node_data_type = World>  // @for_now - For now
// only, we are giving it this default templated version
class World_Tree {
  public:
    typedef World_Node *World_Node_Ptr;
    typedef Display *Display_Ptr;

  private:
    Display_Ptr displayManager; // the displayManager for the verse
    Verse *parent_verse;
    World_Node_Ptr root;
    int16_t num_nodes{0};

  public:
    const Display *access_disp_manager() const;
    Display *access_disp_manager();

    bool initTree();
    void destructTree();

    World_Node_Ptr const getRootNode() const { return this->root; }

    friend class Display;

    World_Tree(Verse *, Display_Ptr displayManager);
    ~World_Tree();
};
