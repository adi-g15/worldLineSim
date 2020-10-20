#pragma once

/*NOTE - world line simulator will inherit from this Square_Matrix to create a worldPlot class, to provide custom functions helpful to the particular implementations of the World class*/

#include "graph_mat_decl.hpp"

template< typename node_type, typename dimen_t = int32_t >
class Square_Matrix: public Graph_Matrix<node_type, dimen_t>{
    public:
        dimen_t getOrder() const{  //order of the matrix
            return this->_total_y_abs;
        }
        void resize(dimen_t new_order){
            Graph_Matrix::resize(new_order, new_order);
        }

    Square_Matrix(): Square_Matrix(1){}
    Square_Matrix(dimen_t order): Graph_Matrix(order, order){

    }

};
