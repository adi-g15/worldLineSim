#pragma once

#include <cstdint>
#include <utility>
// #include <concepts>

#include "graph_box.hpp"

 //udimen_t will be the unit of dimension
template< typename node_type, typename udimen_t=uint32_t>
class graph_Matrix{
    typedef graph_box<node_type> graph_box_type;
    // typedef uint32_t udimen_t;  //dimension unit

private:
    graph_box_type root; // [0,0]

        //pointers to the 3 nodes
    graph_box_type* bl_box;  // bottom_left, [n_row-1][0]
    graph_box_type* br_box;  // bottom_right, [n_row-1][n_col-1]
    graph_box_type* tr_box;  // bottom_right, [0][n_col-1]

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

    std::pair< udimen_t, udimen_t > getDimensions() const;  // returns in form of 

    auto getNumRows() const;
    auto getNumCols() const;

    void displayMat(std::ostream& output_stream = std::cout) const;

    // friend std::ostream& operator<<(std::ostream& os, const graph_Matrix<node_type,udimen_t>&) const{
    //     this->displayMat(os);
    // }

    graph_Matrix();
    graph_Matrix(udimen_t _x_dimen, udimen_t _y_dimen);
    // graph_Matrix(udimen_t _x_dimen, udimen_t _y_dimen, std::function);   //takes a function, that takes in two values (the x and y coord of that point), and give out a value to be alloted to the new box created there
    ~graph_Matrix();
};
