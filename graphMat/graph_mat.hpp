#pragma once

#include "graph_mat_decl.hpp"

//iterates over all the boxes
template< typename node_type, typename dimen_t>
bool Graph_Matrix<node_type, dimen_t>::forRange(graph_box_type* iteratee){ //returns false when iterated all of that of that row
	// @warning "This overload will iterate through ALL BOXES, which may be very large, be sure, if you wanted this"

	//moved to util::range_iterator
	graph_box_type* temp = this->origin;

	return true;
}

template< typename node_type, typename dimen_t>
void Graph_Matrix<node_type, dimen_t>::pushCol(){ // add a column at end
	this->tr_box->right = new graph_box_type(0, this->_total_x_abs);
	this->tr_box->right->left = this->tr_box;

	graph_box_type* prev_new_box = this->tr_box->right;

	// @future- Can use util::range_iterator later, for the same iteration
	graph_box_type* last_col_iter = this->tr_box->down != nullptr ? this->tr_box->down : this->br_box;   //iterator in last column
		// in the else case of ?: so that the next loop doesn't execute in case the matrix contained only 1 row

	dimen_t count{1};
	while( last_col_iter != this->br_box ){ //ideally shouldn't be infinite, since last_col_iter->down should reach the bottom surely
		// we also have to give the previous box, left, right, down pointer to the new Box
		last_col_iter->right = new graph_box_type(count, this->_total_x_abs);
		last_col_iter->right->left = last_col_iter;
		last_col_iter->right->up = prev_new_box;

		prev_new_box->down = last_col_iter->right;
		prev_new_box = last_col_iter->right;
		last_col_iter = last_col_iter->down;

		++count;
	}

	if( this->br_box != this->tr_box ){
		this->br_box->right = new graph_box_type(this->_total_y_abs-1, this->_total_x_abs);
		prev_new_box->down = this->br_box->right;
		this->br_box->right->left = this->br_box;
		this->br_box->right->up = prev_new_box;
	}

	this->tr_box = this->tr_box->right;
	this->br_box = this->br_box->right;    // new bottom right

	++(this->_total_x_abs);
}

template< typename node_type, typename dimen_t>
void Graph_Matrix<node_type, dimen_t>::pushRow(){ // add a row at end
	this->bl_box->down = new graph_box_type(this->_x_min-1, 0);
	this->bl_box->down->up = this->bl_box;

	graph_box_type* prev_new_box = this->bl_box->down;

	// @future- Can use util::range_iterator later, for the same iteration
	graph_box_type* last_row_iter = this->bl_box->right != nullptr ? this->bl_box->right : this->br_box;   //iterator in last column
		// in the else case of ?: so that the next loop doesn't execute in case the matrix contained only 1 row

	dimen_t count{1};
	// the below condition can be replaced with equivalent for loop using _N_Columns
	while( last_row_iter != this->br_box ){ //ideally shouldn't be infinite, since last_row_iter->right should reach the bottomRight surely
		// we also have to give the previous box, left, right, down pointer to the new Box
		last_row_iter->down = new graph_box_type(this->_total_y_abs, count);
		last_row_iter->down->up = last_row_iter;
		last_row_iter->down->left = prev_new_box;

		prev_new_box->right = last_row_iter->down;
		prev_new_box = last_row_iter->down;
		last_row_iter = last_row_iter->right;

		++count;
	}

	if( this->br_box != this->bl_box ){
		this->br_box->down = new graph_box_type(this->_y_min - 1, this->-1);
		prev_new_box->right = this->br_box->down;
		this->br_box->down->up = this->br_box;
		this->br_box->down->left = prev_new_box;
	}

	this->bl_box = this->bl_box->down;
	this->br_box = this->br_box->down;    // new bottom right

	--(_y_min);	// ie. increase in negative
	++(this->_total_y_abs);
}

template< typename node_type, typename dimen_t>
void Graph_Matrix<node_type, dimen_t>::injectCol(){ // add a column at begin
	this->origin.left = new graph_box_type(0,0);
	this->origin.left->right = &(this->origin);

	graph_box_type* prev_new_box = this->origin.right;

	// @future- Can use util::range_iterator later, for the same iteration
	graph_box_type* last_col_iter = this->origin.down != nullptr ? this->origin.down : this->bl_box;   //iterator in last column
		// in the else case of ?: so that the next loop doesn't execute in case the matrix contained only 1 row

	while( last_col_iter != this->bl_box ){ //ideally shouldn't be infinite, since last_col_iter->down should reach the bottom surely
		// we also have to give the previous box, left, right, down pointer to the new Box
		last_col_iter->left = new graph_box_type;
		last_col_iter->left->right = last_col_iter;
		last_col_iter->left->up = prev_new_box;

		prev_new_box->down = last_col_iter->left;
		prev_new_box = last_col_iter->left;
		last_col_iter = last_col_iter->down;
	}

	if( this->bl_box != &(this->origin) ){
		this->bl_box->left = new graph_box_type;
		prev_new_box->down = this->bl_box->left;
		this->bl_box->left->right = this->bl_box;
		this->bl_box->left->up = prev_new_box;
	}

	this->origin = &(this->origin.left);
	this->br_box = this->br_box->left;    // new bottom right

	++(this->_total_x_abs);
}

template< typename node_type, typename dimen_t>
void Graph_Matrix<node_type, dimen_t>::injectRow(){ // add a row at begin
	this->origin.up = new graph_box_type;
	this->origin.up->down = &(this->origin);

	graph_box_type* prev_new_box = this->origin.up;

	graph_box_type* last_row_iter = this->origin.right != nullptr ? this->origin.right : this->tr_box;   //iterator in last column
		// in the else case of ?: so that the next loop doesn't execute in case the matrix contained only 1 row

	// the below condition can be replaced with equivalent for loop using _N_Columns
	while( last_row_iter != this->tr_box ){ //ideally shouldn't be infinite, since last_row_iter->right should reach the bottomRight surely
		// we also have to give the previous box, left, right, down pointer to the new Box
		last_row_iter->up = new graph_box_type;
		last_row_iter->up->down = last_row_iter;
		last_row_iter->up->left = prev_new_box;

		prev_new_box->right = last_row_iter->up;
		prev_new_box = last_row_iter->up;
		last_row_iter = last_row_iter->right;
	}

	if( this->tr_box != &(this->origin) ){
		this->tr_box->up = new graph_box_type;
		prev_new_box->right = this->tr_box->up;
		this->tr_box->up->down = this->tr_box;
		this->tr_box->left->up = prev_new_box;
	}

	this->origin = *(this->origin.up);
	this->tr_box = this->tr_box->up;    // new top right

	++(this->_total_y_abs);
}

template< typename node_type, typename dimen_t>
void Graph_Matrix<node_type, dimen_t>::popCol(){   //remove the last column ( = n_col-1)
	if( this->tr_box == &(this->origin) )   return; //don't pop out the origin

	graph_box_type* temp = this->tr_box;
	this->tr_box = this->tr_box->left;

	// we have to clear the down pointers of this->tr_box here
	while( temp != this->br_box ){
		temp->left->right = nullptr;    // logically temp->left != nullptr, since atleast it will point to an element in the origin 1st column
		temp = temp->down;

		delete temp->up;
	};

	this->br_box = this->br_box->left;
	delete this->br_box->right;
	this->br_box->right = nullptr;

	--(this->_total_x_abs);
}

template< typename node_type, typename dimen_t>
void Graph_Matrix<node_type, dimen_t>::popRow(){   //remove the last row ( = n_row-1)
	if( this->bl_box == &(this->origin) )   return; //don't pop out the origin

	graph_box_type* temp = this->bl_box;
	this->bl_box = this->bl_box->up;    //since this bl_box will be removed

	// we have to clear the right pointers of this->bl_box here
	while( temp != this->br_box ){
		// @CAUTION - SEG FAULT
		temp->up->down = nullptr;    // logically temp->left != nullptr, since atleast it will point to an element in the origin 1st column
		temp = temp->right;

		delete temp->left;
	}

	this->br_box = this->br_box->up;
	delete this->br_box->down;
	this->br_box->down = nullptr;

	--(this->_total_y_abs);
}

template< typename node_type, typename dimen_t>
std::pair< dimen_t, dimen_t > Graph_Matrix<node_type, dimen_t>::getDimensions() const{
	return { this->_total_y_abs, this->_total_x_abs };    //return in form of rows, cols
}

template< typename node_type, typename dimen_t>
auto Graph_Matrix<node_type, dimen_t>::getNumRows() const{
	return this->_total_y_abs;
}

template< typename node_type, typename dimen_t>
auto Graph_Matrix<node_type, dimen_t>::getNumCols() const{
	return this->_total_x_abs;
}

template< typename node_type, typename dimen_t>
void Graph_Matrix<node_type, dimen_t>::displayMat(std::ostream& os) const{
	// @todo - Use util::range_iterator here to iterate through all boxes

	// temporary code
	const graph_box_type* box = &(this->origin);
	const graph_box_type* row_start = &(this->origin);

	while( row_start != nullptr ){
		box = row_start;
		while( box != nullptr ){
			os << box->data << " | ";
			box = box->right;
		}
		os << '\n';
		row_start = row_start->down;
	}
}

template< typename node_type, typename dimen_t>
void Graph_Matrix<node_type, dimen_t>::resize(udimen_t x_dimen, udimen_t y_dimen){
	// @future - Use this->__capacity to, use the nodes, previously allocated only, instead of allocating again

	if( x_dimen == 0 || y_dimen == 0 )	return;	//as for now, don't allow 0 cols or rows, though even later it is less probable to have such behaviour

	// @future - Instead of simply popping, prefer to change size only, utilizing __capacity
	while( y_dimen < this->_total_x_abs ){
		this->popCol();
	}
	while( x_dimen < this->_total_y_abs ){
		this->popRow();
	}

	x_dimen -= this->_total_y_abs;	//x_dimen and y_dimen will now how how many `more` to add
	y_dimen -= this->_total_x_abs;

	dimen_t left_inc, right_inc, down_inc, up_inc;
	up_inc = y_dimen / 2;
	down_inc = y_dimen - y_dimen / 2;
	left_inc = x_dimen / 2;
	right_inc = x_dimen - x_dimen / 2;

	while( down_inc-- > 0 ){  //add x_dimen/2 columns, to bottom
		this->pushRow();
	}
	while( up_inc-- > 0 ){  //add x_dimen-x_dimen/2 cols, above
		this->injectRow();
	}
	while( right_inc-- > 0 ){  //add y_dimen/2 cols, to right
		this->pushCol();
	}
	while( left_inc-- > 0 ){  //add y_dimen-y_dimen/2 cols, to left
		this->injectCol();
	}
}


// ACCESSOR FUNCTIONS //
template< typename node_type, typename dimen_t>
Graph_Box<node_type>* Graph_Matrix<node_type, dimen_t>::operator[](const _coord<dimen_t>& pos){
	// @note to viewer -> You can express your view on whether we should prefer simple [x][y] for position or the graph_position typedefed in graph_box.hpp
	// @todo - call overloaded operator taking graph_position
}

template< typename node_type, typename dimen_t>
const Graph_Box<node_type>* Graph_Matrix<node_type, dimen_t>::operator[](const _coord<dimen_t>& pos) const{
	// @note to viewer -> You can express your view on whether we should prefer simple [x][y] for position or the graph_position typedefed in graph_box.hpp
	// @todo - call overloaded operator taking graph_position
}

template< typename node_type, typename dimen_t>
Graph_Box<node_type>* Graph_Matrix<node_type, dimen_t>::operator[](const graph_position& pos){
	// @note to viewer -> You can express your view on whether we should prefer simple [x][y] for position or the graph_position typedefed in graph_box.hpp

}

template< typename node_type, typename dimen_t>
const Graph_Box<node_type>* Graph_Matrix<node_type, dimen_t>::operator[](const graph_position& pos) const{
	// @note to viewer -> You can express your view on whether we should prefer simple [x][y] for position or the graph_position typedefed in graph_box.hpp

}

// ACCESSOR FUNCTIONS //

// CONSTRUCTORS AND DESTRUCTORS	//

template< typename node_type, typename dimen_t>
Graph_Matrix<node_type, dimen_t>::Graph_Matrix() : Graph_Matrix(1, 1){}

/*
	The origin -> as per the current initialisation of _x_min, _x_max and y_min y_max,
					the origin box is the square with the corners -> (0,0), (0,1), (1,1), (1,0)

					_total_x_abs(1), _total_y_abs(1), _x_min(0), _x_max(1), _y_min(0), _y_max(1), origin(0,0)

				UPDATE: After i have changed all ..._abs ..._min and ..._max to initialise with 0,
					So, each box is not actually a square with four corners, instead it will simple have just one coordinate (similar to a point)
*/

template< typename node_type, typename dimen_t>
																								// @look_later - Here, for now i have initialised _x_min and _x_max as given below, but shouldn't they both be 0,0 ? Same for _y_min and _y_max ?
Graph_Matrix<node_type, dimen_t>::Graph_Matrix(dimen_t _x_dimen, dimen_t _y_dimen) : _total_x_abs(1), _total_y_abs(1), _x_min(0), _x_max(1), _y_min(0), _y_max(1), origin(0,0){
	this->origin.data = decltype(this->origin.data){};	// @caution -> due to this , and maybe code at other places, the `node_type` has to copy-constructible

	this->tl_box = &origin;
	this->bl_box = &origin;
	this->tr_box = &origin;
	this->br_box = &origin;

	while( --(_x_dimen) ){  //add _x_dimen-1 columns (already initialises by one default)
		this->pushRow();
	}
	while( --(_y_dimen) ){  //add _y_dimen-1 columns (already initialises by one default)
		this->pushCol();
	}
}

template< typename node_type, typename dimen_t>
Graph_Matrix<node_type, dimen_t>::~Graph_Matrix(){
	// @todo : May cause problem, when only single element exists... if tried to delete the three pointers from the bottom

	using std::cout; using std::endl;
	while( this->_total_y_abs != 1 ){  //excluding the origin node, which will automatically get destructed, since it's not a pointer
		popRow();   //remove the last row ( = n_row-1)     
	}

	while( this->_total_x_abs != 1 ){  //excluding the origin node, which will automatically get destructed, since it's not a pointer
		popCol();
	}
}

// CONSTRUCTORS AND DESTRUCTORS	//
