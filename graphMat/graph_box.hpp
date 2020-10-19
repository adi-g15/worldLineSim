#pragma once

#include <ostream>

// #include "graph_mat_decl.hpp"

enum Direction: uint8_t{ // intentionally not enum class now
    UP,
    DOWN,
    RIGHT,
    LEFT
};

template< typename node_type >
struct graph_box    //make it non-copyable
{
    public:
        const std::remove_reference_t<node_type>& getData() const{ return this->data; }

        graph_box* get_box(){ return this; }
        const graph_box* get_box() const{ return this; }
        graph_box* get_adj_box( Direction dir ) const{
            switch (dir)
            {
                case Direction::UP: return this->up;
                case Direction::DOWN: return this->down;
                case Direction::RIGHT: return this->right;
                case Direction::LEFT: return this->left;
                default: return nullptr;
            }
        }

        graph_box(){
            this->up = nullptr;
            this->down = nullptr;
            this->left = nullptr;
            this->right = nullptr;
        }

        graph_box(const graph_box<node_type>&) = delete;
        graph_box(const graph_box<node_type>&&) = delete;

        friend std::ostream& operator<<( std::ostream& os, const graph_box& box){
            os << box.getData();
            return os;
        }
        // friend class graph_Matrix<node_type>;

    private:
        node_type data;

        graph_box* right;
        graph_box* left;
        graph_box* up;
        graph_box* down;
        // friend class graph_Matrix<node_type>;

        template<typename,typename> friend class graph_Matrix;    //will make all graph_Matrix friend to this

};
