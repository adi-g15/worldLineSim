#pragma once

#include <ostream>
#include <vector>
#include <utility>

#include "direction.hpp"
#include "../includes/declarations.hpp"

// equivalent to {x,y} in a matrix, this typedef defines the position of a box/point, in an intuitive way (to me atleast :D), and feels more closer to the implementation than to give indices, since here we have directions mostly
typedef std::vector<std::pair< Direction, uint16_t >> graph_position;

template< typename node_type >
struct Graph_Box    //make it non-copyable
{
    typedef int32_t dimen_t;
    typedef _coord<dimen_t> coord_type;

    public:
        Graph_Box* get_box(){ return this; }
        const Graph_Box* get_box() const{ return this; }
        Graph_Box* get_adj_box( Direction dir ) const{
            switch (dir)
            {
                case Direction::UP: return this->up;
                case Direction::DOWN: return this->down;
                case Direction::RIGHT: return this->right;
                case Direction::LEFT: return this->left;
                default: return nullptr;
            }
        }

        Graph_Box(dimen_t x, dimen_t y): Graph_Box(node_type{}, x, y){}
        Graph_Box(const _coord<dimen_t>& coord): Graph_Box(node_type{}, coord.mX, coord.mY){}

        Graph_Box(node_type data, dimen_t x, dimen_t y): data(data), coords(x,y){
            this->up = nullptr;
            this->down = nullptr;
            this->left = nullptr;
            this->right = nullptr;
        }

        Graph_Box(const Graph_Box<node_type>&) = delete;
        Graph_Box(const Graph_Box<node_type>&&) = delete;

        friend std::ostream& operator<<( std::ostream& os, const Graph_Box& box){
            os << box.getData();
            return os;
        }
        // friend class Graph_Matrix<node_type>;

        const coord_type& getCoords(){ return this->coords; }
        const std::remove_reference<node_type>* getData() const{ return this->data; }

    protected:
        node_type data;    /*This has been given as an extension, so that you can add more variables to the graph_box
                            though, note that, you will be able to access using this->data->your_var_name */
        coord_type coords;  // @NOTE - not actually needed, though this maybe used in my implementation of snake

        Graph_Box* right;
        Graph_Box* left;
        Graph_Box* up;
        Graph_Box* down;
        // friend class Graph_Matrix<node_type>;

        // LEARNT - friending a templated class (down below is the `syntax` found, to friend `all` templated versions of Graph_Matrix)
        template<typename,typename> friend class Graph_Matrix;    //will make all Graph_Matrix friend to this

};
