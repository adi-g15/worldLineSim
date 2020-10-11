#pragma once

#include <map>
#include <list>
#include <memory>

template< typename T >
class Tree{
    T& root;    // could have been unique_ptr, but we need a pointer in World for the `current active world`
    std::list< T > nodes;   // we can't have std::list of raw references (though using wrapper should work, but not needed here)

    public:
        Tree(): root(T{}){}
        Tree(T& root): root(root){}

        T& find();  //@todo - should take a lambda
        void insert(T&);
        size_t size();
};
