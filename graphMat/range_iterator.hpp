#pragma once

#include "graph_mat_decl.hpp"

template<typename dimen_t = int32_t>
class range_iterator{   //only for graph_matrix
    private:
        //temporary variables for for range loops
        dimen_t __tmp_forRange_x;
        dimen_t __tmp_forRange_y;

    public:
        range_iterator(){
            this->__tmp_forRange_x = 
        }
        ~range_iterator(){

        }
};


/*
EXAMPLE - 
struct decrementer{
        //temporary variables for for range loops
    uint32_t __tmp_forRange_x;
    uint32_t __tmp_forRange_y;

    uint32_t __start;
    mutable uint32_t __cur;
    uint32_t __end;

    void seed(uint32_t seed){
        __start = seed;
        __cur = __start;
        __end = 0;        
    }

    bool operator()(int* iteratee) const{
        if( __cur == __end ){
            return false;
        }

        -- __cur;
        if( !iteratee ) throw std::invalid_argument("Iteratee must not be a null pointer");
        *iteratee = __cur;
        return true;
    }

    decrementer(int seed){
        this->seed(seed);
        // if( std::abs((long)(__start - __end)) <= std::abs((long)(advancing_func(__start,1) - end)) ){
        //     throw std::overflow_error("For range iteratee will loop infinitely, __start will just keep getting farther to __end");
        // }

        cout << "Constructing "<< endl;
    }
    ~decrementer(){
        cout << "Destructing "<< endl;
    }
};


    decrementer t(6);
    int i;   
    while ( t(&i) )
    {
        cout << i << endl;
    }
    cout << "Out of while"<< endl;

*/