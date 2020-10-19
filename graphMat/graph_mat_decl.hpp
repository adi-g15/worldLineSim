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

// dimen_t will be the unit of dimension
template< typename node_type, typename dimen_t=int32_t>
class Graph_Matrix{
    typedef Graph_Box<node_type> graph_box_type;
    typedef std::make_unsigned<dimen_t> udimen_t;
    // typedef int32_t dimen_t;  //dimension unit

protected:
    graph_box_type origin; // [0,0]; will never change

        //pointers to the 3 nodes
    graph_box_type* tl_box;  // top_left, [n_row-1][0]
    graph_box_type* bl_box;  // bottom_left, [n_row-1][0]
    graph_box_type* tr_box;  // bottom_right, [0][n_col-1]
    graph_box_type* br_box;  // bottom_right, [n_row-1][n_col-1]

    struct {
        graph_box_type* origin;
        graph_box_type* tl_box;  // top_left, [n_row-1][0]
        graph_box_type* bl_box;  // bottom_left, [n_row-1][0]
        graph_box_type* br_box;  // bottom_right, [n_row-1][n_col-1]
        graph_box_type* tr_box;  // bottom_right, [0][n_col-1]
        dimen_t _total_y_abs;
        dimen_t _total_x_abs;
    } __capacity;   //capacity data

    dimen_t _x_min, _x_max;
    dimen_t _y_min, _y_max;

    // @warning - May later change it as well to `dimen_t`, since `if _total_y_abs and _N_cols are used in substraction, it may well cause `wrapping around of the unsigned``
    udimen_t _total_y_abs;   // originally it was named `_N_Rows`, but the name has been changed to adopt to the usual x-y coordinate system, AND for each box to have coordinates, which required updation of all coordinates, like if a row is added, origin(0,0) shifted downwards, so all other coords should change as well
    udimen_t _total_x_abs;  // originally names `_N_Cols`

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

    virtual std::pair< dimen_t, dimen_t > getDimensions() const;  // returns in form of 

    auto getNumRows() const;
    auto getNumCols() const;

        // @note to viewer -> You can express your view on whether we should prefer simple [x][y] for position or the graph_position typedefed in graph_box.hpp
    graph_box_type* operator[](const _coord<dimen_t>&);
    const graph_box_type* operator[](const _coord<dimen_t>&) const;
    graph_box_type* operator[](const graph_position& pos);
    const graph_box_type* operator[](const graph_position& pos) const;

    // @future - These two to be implemented later and should utilize this->__capacity
    // void reserve(int _x_dimen, int _y_dimen);
    // void shrink_to_fit();

    // virtual void resize(dimen_t x_dimen, dimen_t y_dimen, Direction a, ...);   // @future - resize with variadic arguments, that takes directions after the new dimensions
    virtual void resize(udimen_t x_dimen, udimen_t y_dimen);

    void displayMat(std::ostream& output_stream = std::cout) const;

    // friend std::ostream& operator<<(std::ostream& os, const Graph_Matrix<node_type,dimen_t>&) const{
    //     this->displayMat(os);
    // }

    Graph_Matrix();
    Graph_Matrix(dimen_t _x_dimen, dimen_t _y_dimen);
    // Graph_Matrix(dimen_t _x_dimen, dimen_t _y_dimen, std::function);   //takes a function, that takes in two values (the x and y coord of that point), and give out a value to be alloted to the new box created there
    ~Graph_Matrix();
};
