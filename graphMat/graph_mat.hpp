#pragma once

#include "graph_mat_decl.hpp"

//iterates over all the boxes
template< typename node_type, typename udimen_t>
bool graph_Matrix<node_type, udimen_t>::forRange(graph_box_type* iteratee){ //returns false when iterated all of that of that row
    // @warning "This overload will iterate through ALL BOXES, which may be very large, be sure, if you wanted this"

    //moved to util::range_iterator
    graph_box_type* temp = this->root;

    return true;
}

template< typename node_type, typename udimen_t>
void graph_Matrix<node_type, udimen_t>::pushCol(){ // add a column at end
    this->tr_box->right = new graph_box_type;
    this->tr_box->right->left = this->tr_box;

    graph_box_type* prev_new_box = this->tr_box->right;

    // @future- Can use util::range_iterator later, for the same iteration
    graph_box_type* last_col_iter = this->tr_box->down != nullptr ? this->tr_box->down : this->br_box;   //iterator in last column
        // in the else case of ?: so that the next loop doesn't execute in case the matrix contained only 1 row

    while( last_col_iter != this->br_box ){ //ideally shouldn't be infinite, since last_col_iter->down should reach the bottom surely
        // we also have to give the previous box, left, right, down pointer to the new Box
        last_col_iter->right = new graph_box_type;
        last_col_iter->right->left = last_col_iter;
        last_col_iter->right->up = prev_new_box;

        prev_new_box->down = last_col_iter->right;
        prev_new_box = last_col_iter->right;
        last_col_iter = last_col_iter->down;
    }

    if( this->br_box != this->tr_box ){
        this->br_box->right = new graph_box_type;
        prev_new_box->down = this->br_box->right;
        this->br_box->right->left = this->br_box;
        this->br_box->right->up = prev_new_box;
    }

    this->tr_box = this->tr_box->right;
    this->br_box = this->br_box->right;    // new bottom right

    ++(this->_N_Cols);
}

template< typename node_type, typename udimen_t>
void graph_Matrix<node_type, udimen_t>::pushRow(){ // add a row at end
    this->bl_box->down = new graph_box_type;
    this->bl_box->down->up = this->bl_box;

    graph_box_type* prev_new_box = this->bl_box->down;

    // @future- Can use util::range_iterator later, for the same iteration
    graph_box_type* last_row_iter = this->bl_box->right != nullptr ? this->bl_box->right : this->br_box;   //iterator in last column
        // in the else case of ?: so that the next loop doesn't execute in case the matrix contained only 1 row

    // the below condition can be replaced with equivalent for loop using _N_Columns
    while( last_row_iter != this->br_box ){ //ideally shouldn't be infinite, since last_row_iter->right should reach the bottomRight surely
        // we also have to give the previous box, left, right, down pointer to the new Box
        last_row_iter->down = new graph_box_type;
        last_row_iter->down->up = last_row_iter;
        last_row_iter->down->left = prev_new_box;

        prev_new_box->right = last_row_iter->down;
        prev_new_box = last_row_iter->down;
        last_row_iter = last_row_iter->right;
    }

    if( this->br_box != this->bl_box ){
        this->br_box->down = new graph_box_type;
        prev_new_box->right = this->br_box->down;
        this->br_box->down->up = this->br_box;
        this->br_box->down->left = prev_new_box;
    }

    this->bl_box = this->bl_box->down;
    this->br_box = this->br_box->down;    // new bottom right

    ++(this->_N_Rows);
}

template< typename node_type, typename udimen_t>
void graph_Matrix<node_type, udimen_t>::injectCol(){ // add a column at begin
    // @todo

    ++(this->_N_Rows);
}

template< typename node_type, typename udimen_t>
void graph_Matrix<node_type, udimen_t>::injectRow(){ // add a row at begin
    // @todo

    ++(this->_N_Cols);
}

template< typename node_type, typename udimen_t>
void graph_Matrix<node_type, udimen_t>::popCol(){   //remove the last column ( = n_col-1)
    if( this->tr_box == &(this->root) )   return; //don't pop out the root

    graph_box_type* temp = this->tr_box;
    this->tr_box = this->tr_box->left;

    // we have to clear the down pointers of this->tr_box here
    while( temp != this->br_box ){
        temp->left->right = nullptr;    // logically temp->left != nullptr, since atleast it will point to an element in the root 1st column
        temp = temp->down;

        delete temp->up;
    };

    this->br_box = this->br_box->left;
    delete this->br_box->right;
    this->br_box->right = nullptr;

    --(this->_N_Cols);
}

template< typename node_type, typename udimen_t>
void graph_Matrix<node_type, udimen_t>::popRow(){   //remove the last row ( = n_row-1)
    if( this->bl_box == &(this->root) )   return; //don't pop out the root

    graph_box_type* temp = this->bl_box;
    this->bl_box = this->bl_box->up;    //since this bl_box will be removed

    // we have to clear the right pointers of this->bl_box here
     while( temp != this->br_box ){
            // @CAUTION - SEG FAULT
        temp->up->down = nullptr;    // logically temp->left != nullptr, since atleast it will point to an element in the root 1st column
        temp = temp->right;

        delete temp->left;
    }

    this->br_box = this->br_box->up;
    delete this->br_box->down;
    this->br_box->down = nullptr;

    --(this->_N_Rows);
}

template< typename node_type, typename udimen_t>
std::pair< udimen_t, udimen_t > graph_Matrix<node_type, udimen_t>::getDimensions() const{
    return { this->_N_Rows, this->_N_Cols };    //return in form of rows, cols
}

template< typename node_type, typename udimen_t>
auto graph_Matrix<node_type, udimen_t>::getNumRows() const{
    return this->_N_Rows;
}

template< typename node_type, typename udimen_t>
auto graph_Matrix<node_type, udimen_t>::getNumCols() const{
    return this->_N_Cols;
}

template< typename node_type, typename udimen_t>
void graph_Matrix<node_type, udimen_t>::displayMat(std::ostream& os) const{
    // @todo - Use util::range_iterator here to iterate through all boxes

    // temporary code
    const graph_box_type* box = &(this->root);
    const graph_box_type* row_start = &(this->root);

    while ( row_start != nullptr )
    {
        box = row_start;
        while (box != nullptr)
        {
            os << box->data << " | ";
            box = box->right;       
        }
        os << '\n';
        row_start = row_start->down;
    }
}

template< typename node_type, typename udimen_t>
graph_Matrix<node_type, udimen_t>::graph_Matrix() : graph_Matrix(1, 1){}

template< typename node_type, typename udimen_t>
graph_Matrix<node_type, udimen_t>::graph_Matrix(udimen_t _x_dimen, udimen_t _y_dimen) : _N_Cols(1), _N_Rows(1){
    this->root.data = decltype(this->root.data){};

    this->bl_box = &root;
    this->tr_box = &root;
    this->br_box = &root;

    while( --(_x_dimen) ){  //add _x_dimen-1 columns (already initialises by one default)
        this->pushRow();
    }
    while( --(_y_dimen) ){  //add _y_dimen-1 columns (already initialises by one default)
        this->pushCol();
    }
}

template< typename node_type, typename udimen_t>
graph_Matrix<node_type, udimen_t>::~graph_Matrix(){
    // @todo : May cause problem, when only single element exists... if tried to delete the three pointers from the bottom

    using std::cout; using std::endl;
    while( this->_N_Rows != 1 ){  //excluding the root node, which will automatically get destructed, since it's not a pointer
        cout<<"here1 " << endl;
        popRow();   //remove the last row ( = n_row-1)     
        cout<<"enmd1"<<endl;
    }

    while( this->_N_Cols != 1 ){  //excluding the root node, which will automatically get destructed, since it's not a pointer
        cout<<"here2 " << endl;
        popCol();
        cout<<"end2"<<endl;
    }
}
