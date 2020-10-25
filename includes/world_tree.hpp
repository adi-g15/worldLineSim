#pragma once

#include "util/tree.hpp"
#include <list>
#include <queue>

/*
NOTE - This time, i will first complete World_Tree class from start, and then what seems to be common to other trees, i will slowly shift it to the tree class, and then all those properties are to inhgherited in the Wrd_tree class from Tree class
*/

// template<typename data_type>
struct World_Node{
    typedef World_Node* World_Node_Ptr;

    World data;
    _timePoint latest_time;  //time at which it has been paused

    World_Node_Ptr left_node, right_node;   // each node will have at max two childs
    std::mutex node_mutex;  // @note - Try to have this need removed, by managing edge cases where itis need buyt may not be actually needed

public:
    const World_Ptr get_world() const{
        std::scoped_lock s(node_mutex);
        return &(this->data);
    }
    World_Node() = delete;
    World_Node(World&) = delete;

    // @note - Be sure you have ALL respective arguments as taken by the World class constructor, since the node itself will need them to construct a new world
    World_Node( World_Ptr old_world , _timePoint t ): data(old_world, t){
        this->latest_time = data.currentTime;  //initially
    }
};

// it will actuall ystore dynamiucally allocated objects, not just there pointers
// template<typename node_data_type = World>  // @for_now - For now only, we are giving it this default templated version
class World_Tree : public Tree<World>{
    // typedef Tree_Node* Tree_Node_Ptr;
    typedef World_Node* World_Node_Ptr;
    // typedef _tree_node<World>* World_Node_Ptr;
private:
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
                __latest_world_node->latest_time = __latest_world_node->get_world()->currentTime;
            }
    } _fast_access_data;    // temporary data for fast access, to currently running world

public:
    World_Tree() : root(nullptr), num_nodes(0){}
    World_Tree(World& root_data) : root(new World_Node(root_data)), num_nodes(1){}
    World_Tree(World&& root_data) : root(new World_Node(root_data)), num_nodes(1){}
    ~World_Tree(){
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
