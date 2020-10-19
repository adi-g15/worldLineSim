#pragma once

/* @NOTE- 
 * This class is originally meant to be inherited to from, to provide implementations as needed per your needs
 * THOUGH, IF YOU ALLOCATE MEMORY IN IT, BE SURE TO TAKE CARE OF THAT AT DESTRUCTION, I CURRENTLY DONT KNOW MUCH OF VIRTUAL DESTRUCTORS, WILL MODIFY AS NEEDED
 * I appreciate help regarding it (you may find this vague for me trying all this wuthout much knowledge of virtual destructors, i have read of them, but never used)
*/

#include <cstdint>
#include <utility>
// #include <concepts>

#include "graph_box.hpp"

    // @limitation (wrt vector<vector>) - Currently you can't get back the nth row, say mat[4], isn't valid, though can be implemented by simply returning, mat[4][0] which will logically be a linked list

// udimen_t will be the unit of dimension
template< typename node_type, typename udimen_t=uint32_t>
class Graph_Matrix{
    typedef Graph_Box<node_type> graph_box_type;
    // typedef uint32_t udimen_t;  //dimension unit

protected:
    graph_box_type root; // [0,0]

        //pointers to the 3 nodes
    graph_box_type* bl_box;  // bottom_left, [n_row-1][0]
    graph_box_type* br_box;  // bottom_right, [n_row-1][n_col-1]
    graph_box_type* tr_box;  // bottom_right, [0][n_col-1]

    struct {
        graph_box_type* root;
        graph_box_type* bl_box;  // bottom_left, [n_row-1][0]
        graph_box_type* br_box;  // bottom_right, [n_row-1][n_col-1]
        graph_box_type* tr_box;  // bottom_right, [0][n_col-1]
        udimen_t _N_Rows;   // expecting the dimension not greater than a 32 bit number maximum
        udimen_t _N_Cols;
    } __capacity;   //capacity data

    udimen_t _N_Rows;   // expecting the dimension not greater than a 32 bit number maximum
    udimen_t _N_Cols;

public:
        // @deprecated+incomplete -> use util::range_iterator for now
        // @note - util::range_iterator has ben postponed for now, will be implemented as needed
    bool forRange(graph_box_type* iteratee); //returns false when iterated all of that of that row

    void pushCol(); // add a column at end
    void pushRow(); // add a row at end
    void injectCol(); // add a column at begin
    void injectRow(); // add a row at begin

    void popCol(); // add a column at end
    void popRow(); // add a row at end

    virtual std::pair< udimen_t, udimen_t > getDimensions() const;  // returns in form of 

    auto getNumRows() const;
    auto getNumCols() const;

        // @note to viewer -> You can express your view on whether we should prefer simple [x][y] for position or the graph_position typedefed in graph_box.hpp
    graph_box_type* operator[](const _coord<udimen_t>&);
    const graph_box_type* operator[](const _coord<udimen_t>&) const;
    graph_box_type* operator[](const graph_position& pos);
    const graph_box_type* operator[](const graph_position& pos) const;

    // @future - These two to be implemented later and should utilize this->__capacity
    // void reserve(int _x_dimen, int _y_dimen);
    // void shrink_to_fit();

    // virtual void resize(udimen_t x_dimen, udimen_t y_dimen, Direction a, ...);   // @future - resize with variadic arguments, that takes directions after the new dimensions
    virtual void resize(udimen_t x_dimen, udimen_t y_dimen);

    void displayMat(std::ostream& output_stream = std::cout) const;

    // friend std::ostream& operator<<(std::ostream& os, const Graph_Matrix<node_type,udimen_t>&) const{
    //     this->displayMat(os);
    // }

    Graph_Matrix();
    Graph_Matrix(udimen_t _x_dimen, udimen_t _y_dimen);
    // Graph_Matrix(udimen_t _x_dimen, udimen_t _y_dimen, std::function);   //takes a function, that takes in two values (the x and y coord of that point), and give out a value to be alloted to the new box created there
    ~Graph_Matrix();
};
