#pragma once

#include "graph_box.hpp"
#include <iostream>

using std::clog, std::endl;

    // for logging mainly (not literally 'tests' though)
namespace test{
    namespace logger{
        template<typename node_dtype>
        bool __IsConncted( const Graph_Box<node_dtype>* from, const Graph_Box<node_dtype>* to , Direction dir ){
            const Graph_Box<node_dtype>* temp{ from };

            bool boolean{false}, b2{false};
            for (int i = 0; temp!=nullptr && i < 100; i++){
                if( temp == to ){
                    clog<<"Connected"<<endl;
                    boolean = true;
                    break;
                }
                temp = temp->get_adj_box(dir);
            }

            dir = test::__invertDirection(dir);
            temp = to;

            for (int i = 0; temp!=nullptr && i < 100; i++){
                if( temp == from ){
                    clog<<"Connected Reversed too"<<endl;
                    b2 = true;
                    break;
                }
                temp = temp->get_adj_box(dir);
            }

            if(!boolean || !b2) clog<<"Not Connected"<<endl;
            return boolean && b2;
        }
    }

    Direction __invertDirection( Direction dir ){
        switch (dir){
            case UP:    return DOWN;
            case DOWN:    return UP;
            case RIGHT:    return LEFT;
            case LEFT:    return RIGHT;
        }
        throw std::invalid_argument("Direction invalid");
    }

}
