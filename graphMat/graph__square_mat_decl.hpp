#pragma once

/*NOTE - world line simulator will inherit from this Square_Matrix to create a worldPlot class, to provide custom functions helpful to the particular implementations of the World class*/

#include "graph_mat_decl.hpp"

template< typename node_type, typename udimen_t = uint32_t >
class Square_Matrix: public Graph_Matrix<node_type, udimen_t>{
    public:
        udimen_t getOrder() const{  //order of the matrix
            return this->_N_Rows;
        }
        void resize(udimen_t new_order) override{
            this->resize(new_order, new_order);
        }

};
