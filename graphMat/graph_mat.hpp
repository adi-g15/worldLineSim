#pragma once

#include "graph_mat_decl.hpp"

#define DEBUG_MODE false	/*turn on to get more logging*/

#if DEBUG_MODE
#include "test_util/node_conn.hpp"
#endif

//iterates over all the boxes
template< typename node_dtype, typename dimen_t>
bool Graph_Matrix<node_dtype, dimen_t>::forRange(graph_box_type* iteratee){ //returns false when iterated all of that of that row
	// @warning "This overload will iterate through ALL BOXES, which may be very large, be sure, if you wanted this"

	//moved to util::range_iterator
	graph_box_type* temp = this->origin;

	return true;
}

template< typename node_dtype, typename dimen_t>
void Graph_Matrix<node_dtype, dimen_t>::pushCol(){ // add a column at end
	this->tr_box->right = new graph_box_type(this->_x_max+1, this->_y_max);
	this->tr_box->right->left = this->tr_box;

	graph_box_type* prev_new_box = this->tr_box->right;

	// @future- Can use util::range_iterator later, for the same iteration
	graph_box_type* last_col_iter = this->tr_box->down != nullptr ? this->tr_box->down : this->br_box;   //iterator in last column
		// in the else case of ?: so that the next loop doesn't execute in case the matrix contained only 1 row

	dimen_t count{1};
	while( last_col_iter != this->br_box ){ //ideally shouldn't be infinite, since last_col_iter->down should reach the bottom surely
		// we also have to give the previous box, left, right, down pointer to the new Box
		last_col_iter->right = new graph_box_type(this->_x_max+1, this->_y_max - count);
		last_col_iter->right->left = last_col_iter;
		last_col_iter->right->up = prev_new_box;

		prev_new_box->down = last_col_iter->right;
		prev_new_box = last_col_iter->right;
		last_col_iter = last_col_iter->down;

		++count;
	}

	if( this->br_box != this->tr_box ){
		this->br_box->right = new graph_box_type(this->_x_max+1, this->_y_min);
		prev_new_box->down = this->br_box->right;
		prev_new_box->down->up = prev_new_box;
		this->br_box->right->left = this->br_box;
		this->br_box->right->up = prev_new_box;
	}

	this->tr_box = this->tr_box->right;
	this->br_box = this->br_box->right;    // new bottom right

	++(this->_x_max);
	++(this->_total_x_abs);
}

template< typename node_dtype, typename dimen_t>
void Graph_Matrix<node_dtype, dimen_t>::pushRow(){ // add a row at downward end
	this->bl_box->down = new graph_box_type(this->_x_min, this->_y_min-1);
	this->bl_box->down->up = this->bl_box;

	graph_box_type* prev_new_box = this->bl_box->down;

	// @future- Can use util::range_iterator later, for the same iteration
	graph_box_type* last_row_iter = this->bl_box->right != nullptr ? this->bl_box->right : this->br_box;   //iterator in last column
		// in the else case of ?: so that the next loop doesn't execute in case the matrix contained only 1 row

	dimen_t count{1};
	// the below condition can be replaced with equivalent for loop using _N_Columns
	while( last_row_iter != this->br_box ){ //ideally shouldn't be infinite, since last_row_iter->right should reach the bottomRight surely
		// we also have to give the previous box, left, right, down pointer to the new Box
		last_row_iter->down = new graph_box_type(this->_x_min + count, this->_y_min-1);
		last_row_iter->down->up = last_row_iter;
		last_row_iter->down->left = prev_new_box;

		prev_new_box->right = last_row_iter->down;
		prev_new_box = last_row_iter->down;
		last_row_iter = last_row_iter->right;

		++count;
	}

	if( this->br_box != this->bl_box ){
		this->br_box->down = new graph_box_type(this->_x_max, this->_y_min-1);
		prev_new_box->right = this->br_box->down;
		prev_new_box->right->left = prev_new_box;
		this->br_box->down->up = this->br_box;
		this->br_box->down->left = prev_new_box;
	}

	this->bl_box = this->bl_box->down;
	this->br_box = this->br_box->down;    // new bottom right

	--(this->_y_min);	// ie. increase in negative
	++(this->_total_y_abs);
}

template< typename node_dtype, typename dimen_t>
void Graph_Matrix<node_dtype, dimen_t>::injectCol(){ // add a column at leftmost
	this->tl_box->left = new graph_box_type(this->_x_min-1, this->_y_max);
	this->tl_box->left->right = this->tl_box;

	graph_box_type* prev_new_box = this->tl_box->left;	//the latest new box

	// @future- Can use util::range_iterator later, for the same iteration
	graph_box_type* last_col_iter = this->tl_box->down != nullptr ? this->tl_box->down : this->bl_box;   //iterator in last column
		// in the else case of ?: so that the next loop doesn't execute in case the matrix contained only 1 row

	dimen_t count{1};
	#if DEBUG_MODE
		test::logger::__IsConncted(this->tl_box, this->bl_box, DOWN);
		test::logger::__IsConncted(last_col_iter, this->bl_box, DOWN);
	#endif
	// @CAUTION - Seg Fault here, Cause-> tl_box and bl_box aren't connected in a up to down fashion
	while( last_col_iter != this->bl_box ){ //ideally shouldn't be infinite, since last_col_iter->down should reach the bottom surely
		// we also have to give the previous box, left, right, down pointer to the new Box
		last_col_iter->left = new graph_box_type(this->_x_min-1, this->_y_max - count);
		last_col_iter->left->right = last_col_iter;
		last_col_iter->left->up = prev_new_box;

		prev_new_box->down = last_col_iter->left;
		prev_new_box = last_col_iter->left;
		last_col_iter = last_col_iter->down;

		++count;
	}

	#if DEBUG_MODE
		test::logger::__IsConncted(this->tl_box, this->bl_box, DOWN);
	#endif
	if( this->bl_box != this->tl_box ){
		this->bl_box->left = new graph_box_type(this->_x_min-1, this->_y_min );
		prev_new_box->down = this->bl_box->left;
		prev_new_box->down->up = prev_new_box;
		this->bl_box->left->right = this->bl_box;
		this->bl_box->left->up = prev_new_box;
	}

	this->tl_box = this->tl_box->left;
	this->bl_box = this->bl_box->left;    // new bottom right

	--(this->_x_min);	//increment in negative side
	++(this->_total_x_abs);
}

template< typename node_dtype, typename dimen_t>
void Graph_Matrix<node_dtype, dimen_t>::injectRow(){ // add a row at begin
	this->tl_box->up = new graph_box_type(this->_x_min, this->_y_max+1);
	this->tl_box->up->down = this->tl_box;

	graph_box_type* prev_new_box = this->tl_box->up;	//the latest new box

	graph_box_type* last_row_iter = this->tl_box->right != nullptr ? this->tl_box->right : this->tr_box;   //iterator in last column
		// in the else case of ?: so that the next loop doesn't execute in case the matrix contained only 1 row

	dimen_t count{1};
	// the below condition can be replaced with equivalent for loop using _N_Columns
	while( last_row_iter != this->tr_box ){ //ideally shouldn't be infinite, since last_row_iter->right should reach the bottomRight surely
		// we also have to give the previous box, left, right, down pointer to the new Box
		last_row_iter->up = new graph_box_type(this->_x_min+count, this->_y_max+1);
		last_row_iter->up->down = last_row_iter;
		last_row_iter->up->left = prev_new_box;

		prev_new_box->right = last_row_iter->up;
		prev_new_box = last_row_iter->up;
		last_row_iter = last_row_iter->right;

		++count;
	}

	if( this->tr_box != this->tl_box ){	// ie. row was not having only one element, since in that case we already did a new graph_box_type
		this->tr_box->up = new graph_box_type(this->_x_max, this->_y_max + 1);
		prev_new_box->right = this->tr_box->up;
		prev_new_box->right->left = prev_new_box;
		this->tr_box->up->down = this->tr_box;
		this->tr_box->left->up = prev_new_box;
	}

	this->tl_box = this->tl_box->up;
	this->tr_box = this->tr_box->up;    // new top right

	++(this->_y_max);
	++(this->_total_y_abs);
}

template< typename node_dtype, typename dimen_t>
void Graph_Matrix<node_dtype, dimen_t>::popRow_upper(){   //remove the uppermost row ( = y_max)
	if( this->tl_box == this->bl_box )   return; //don't pop out the origin row (y=0)

	graph_box_type* temp = this->tl_box;
	this->tl_box = this->tl_box->down;    //since this bl_box will be removed

	// we have to clear the right pointers of this->bl_box here
	while( temp != this->tr_box ){
		temp->down->up = nullptr;    // logically temp->left != nullptr, since atleast it will point to an element in the origin 1st column
		temp = temp->right;

		delete temp->left;
	}

	this->tr_box = this->tr_box->down;
	delete this->tr_box->up;
	this->tr_box->up = nullptr;

	--(this->_y_max);
	--(this->_total_y_abs);
}

template< typename node_dtype, typename dimen_t>
void Graph_Matrix<node_dtype, dimen_t>::popCol_left(){   //remove the leftmost column; INTERNAL but can be used
	if( this->tl_box == this->tr_box )   return; //don't pop out the origin column (x=0)

	graph_box_type* temp = this->tl_box;
	this->tl_box = this->tl_box->right;

	// we have to clear the down pointers of this->tr_box here
	while( temp != this->bl_box ){
		temp->right->left = nullptr;
		temp = temp->down;

		delete temp->up;
	};

	this->bl_box = this->bl_box->right;
	delete this->bl_box->left;
	this->bl_box->left = nullptr;

	++(this->_x_min);	//decrement in negative
	--(this->_total_x_abs);
}

template< typename node_dtype, typename dimen_t>
void Graph_Matrix<node_dtype, dimen_t>::popCol(){   //by default, removes the rightmost column ( = x_max ), if not available calls popCol_front() to try to remove the leftmost column, so it's safe to just use this function only
	if( this->tr_box == this->tl_box )   return; //don't pop out the origin column (x=0)

	if( this->_x_max == 0 ) return this->popCol_left();

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

	--(this->_x_max);
	--(this->_total_x_abs);
}

template< typename node_dtype, typename dimen_t>
void Graph_Matrix<node_dtype, dimen_t>::popRow(){   //remove the last row ( = n_row-1)
	if( this->tl_box == this->bl_box )   return; //don't pop out the origin row (y=0)

	if( this->_y_min == 0 )	return this->popRow_upper();

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

	++(this->_y_min);	//decrement in negative side
	--(this->_total_y_abs);
}

template< typename node_dtype, typename dimen_t>
std::pair< std::make_unsigned_t<dimen_t>, std::make_unsigned_t<dimen_t> > Graph_Matrix<node_dtype, dimen_t>::getDimensions() const{
	return { this->_total_y_abs, this->_total_x_abs };    //return in form of rows, cols
}

template< typename node_dtype, typename dimen_t>
auto Graph_Matrix<node_dtype, dimen_t>::getNumRows() const{
	return this->_total_y_abs;
}

template< typename node_dtype, typename dimen_t>
auto Graph_Matrix<node_dtype, dimen_t>::getNumCols() const{
	return this->_total_x_abs;
}

template< typename node_dtype, typename dimen_t>
void Graph_Matrix<node_dtype, dimen_t>::displayMat(std::ostream& os) const{
	// @todo - Use util::range_iterator here to iterate through all boxes

	// temporary code
	const graph_box_type* box = this->tl_box;
	const graph_box_type* row_start = this->tl_box;

	while( row_start != nullptr ){
		box = row_start;
		while( box != nullptr ){
			os << box->coords << " | ";
			box = box->right;
		}
		os << '\n';
		row_start = row_start->down;
	}
}

template< typename node_dtype, typename dimen_t>
void Graph_Matrix<node_dtype, dimen_t>::resize(dimen_t _num_rows, dimen_t _num_cols){
	// @future - Use this->__capacity to, use the nodes, previously allocated only, instead of allocating again
	if( _num_rows < 0 || _num_cols < 0 ){
		throw std::length_error("Cannot have a graph with negative dimensions");
	}

	if( _num_cols == 0 || _num_rows == 0 )	return;	//as for now, don't allow 0 cols or rows, though even later it is less probable to have such behaviour

	--_num_cols;	--_num_rows;	//to adapt to the usual coordinate system, so that if user asks to resize to 1x1, program understands as 0x0 which is the origin

	// @future - Instead of simply popping, prefer to change size only, utilizing __capacity
	while( _num_rows < this->_total_y_abs ){
		this->popRow();
	}

#if DEBUG_MODE
	test::logger::__IsConncted(tl_box, bl_box, DOWN);
	test::logger::__IsConncted(tl_box, tr_box, RIGHT);
	test::logger::__IsConncted(tr_box, br_box, DOWN);
	test::logger::__IsConncted(bl_box, br_box, RIGHT);
#endif

	while( _num_cols < this->_total_x_abs ){
		this->popCol();
	}

#if DEBUG_MODE
	test::logger::__IsConncted(tl_box, bl_box, DOWN);
	test::logger::__IsConncted(tl_box, tr_box, RIGHT);
	test::logger::__IsConncted(tr_box, br_box, DOWN);
	test::logger::__IsConncted(bl_box, br_box, RIGHT);
#endif

	_num_cols -= this->_total_x_abs;	//_num_cols and _num_rows will now how how many `more` to add
	_num_rows -= this->_total_y_abs;

	dimen_t left_inc, right_inc, down_inc, up_inc;
	up_inc = _num_rows / 2;
	down_inc = _num_rows - _num_rows / 2;
	left_inc = _num_cols / 2;
	right_inc = _num_cols - _num_cols / 2;

#if DEBUG_MODE
	this->displayMat();
	std::cout << std::endl;
#endif
	while( down_inc-- > 0 ){  //add _num_cols/2 columns, to bottom
		this->pushRow();
	}
#if DEBUG_MODE
	test::logger::__IsConncted(tl_box, bl_box, DOWN);
	test::logger::__IsConncted(tl_box, tr_box, RIGHT);
	test::logger::__IsConncted(tr_box, br_box, DOWN);
	test::logger::__IsConncted(bl_box, br_box, RIGHT);
#endif

#if DEBUG_MODE
	this->displayMat();
	std::cout << std::endl;
#endif
	while( up_inc-- > 0 ){  //add _num_cols-_num_cols/2 cols, above
		this->injectRow();
	}
#if DEBUG_MODE
	test::logger::__IsConncted(tl_box, bl_box, DOWN);
	test::logger::__IsConncted(tl_box, tr_box, RIGHT);
	test::logger::__IsConncted(tr_box, br_box, DOWN);
	test::logger::__IsConncted(bl_box, br_box, RIGHT);
#endif

#if DEBUG_MODE
	this->displayMat();
	std::cout << std::endl;
#endif
	while( right_inc-- > 0 ){  //add _num_rows/2 cols, to right
		this->pushCol();
	}

#if DEBUG_MODE
	test::logger::__IsConncted(tl_box, bl_box, DOWN);
	test::logger::__IsConncted(tl_box, tr_box, RIGHT);
	test::logger::__IsConncted(tr_box, br_box, DOWN);
	test::logger::__IsConncted(bl_box, br_box, RIGHT);
#endif
#if DEBUG_MODE
	this->displayMat();
	std::cout << std::endl;
#endif
	while( left_inc-- > 0 ){  //add _num_rows-_num_rows/2 cols, to left
		this->injectCol();
	}
#if DEBUG_MODE
	this->displayMat();
	std::cout << std::endl;
#endif

#if DEBUG_MODE
	test::logger::__IsConncted(tl_box, bl_box, DOWN);
	test::logger::__IsConncted(tl_box, tr_box, RIGHT);
	test::logger::__IsConncted(tr_box, br_box, DOWN);
	test::logger::__IsConncted(bl_box, br_box, RIGHT);
#endif
}


// ACCESSOR FUNCTIONS //
template< typename node_dtype, typename dimen_t>
Graph_Box<node_dtype>* Graph_Matrix<node_dtype, dimen_t>::operator[](const _coord<dimen_t>& pos){
	// @note to viewer -> You can express your view on whether we should prefer simple [x][y] for position or the graph_position typedefed in graph_box.hpp
	// @todo - call overloaded operator taking graph_position
}

template< typename node_dtype, typename dimen_t>
const Graph_Box<node_dtype>* Graph_Matrix<node_dtype, dimen_t>::operator[](const _coord<dimen_t>& pos) const{
	// @note to viewer -> You can express your view on whether we should prefer simple [x][y] for position or the graph_position typedefed in graph_box.hpp
	// @todo - call overloaded operator taking graph_position
}

template< typename node_dtype, typename dimen_t>
Graph_Box<node_dtype>* Graph_Matrix<node_dtype, dimen_t>::operator[](const graph_position& pos){
	// @note to viewer -> You can express your view on whether we should prefer simple [x][y] for position or the graph_position typedefed in graph_box.hpp

}

template< typename node_dtype, typename dimen_t>
const Graph_Box<node_dtype>* Graph_Matrix<node_dtype, dimen_t>::operator[](const graph_position& pos) const{
	// @note to viewer -> You can express your view on whether we should prefer simple [x][y] for position or the graph_position typedefed in graph_box.hpp

}

// ACCESSOR FUNCTIONS //

// CONSTRUCTORS AND DESTRUCTORS	//

template< typename node_dtype, typename dimen_t>
Graph_Matrix<node_dtype, dimen_t>::Graph_Matrix() : Graph_Matrix(1, 1){}

/*
	The origin -> as per the current initialisation of _x_min, _x_max and y_min y_max,
					the origin box is the square with the corners -> (0,0), (0,1), (1,1), (1,0)

					_total_x_abs(1), _total_y_abs(1), _x_min(0), _x_max(1), _y_min(0), _y_max(1), origin(0,0)

				UPDATE: After i have changed all ..._abs ..._min and ..._max to initialise with 0,
					So, each box is not actually a square with four corners, instead it will simple have just one coordinate (similar to a point)
*/

template< typename node_dtype, typename dimen_t>
																								// @look_later - Here, for now i have initialised _x_min and _x_max as given below, but shouldn't they both be 0,0 ? Same for _y_min and _y_max ?
Graph_Matrix<node_dtype, dimen_t>::Graph_Matrix(dimen_t _num_rows, dimen_t _num_cols) : _total_x_abs(0), _total_y_abs(0), _x_min(0), _x_max(0), _y_min(0), _y_max(0), origin(0,0){
	if( _num_rows < 0 || _num_cols < 0 ){
		throw std::length_error("Cannot have a graph with negative dimensions");
	}

	this->origin.data = decltype(this->origin.data){};	// @caution -> due to this , and maybe code at other places, the `node_dtype` has to copy-constructible

	this->tl_box = &origin;
	this->bl_box = &origin;
	this->tr_box = &origin;
	this->br_box = &origin;

	while( --(_num_rows) ){  //add _num_rows-1 columns (already initialises by one default)
		this->pushRow();
	}
	while( --(_num_cols) ){  //add _num_cols-1 columns (already initialises by one default)
		this->pushCol();
	}

#if DEBUG_MODE
	test::logger::__IsConncted(tl_box, bl_box, DOWN);
	test::logger::__IsConncted(tl_box, tr_box, RIGHT);
	test::logger::__IsConncted(tr_box, br_box, DOWN);
	test::logger::__IsConncted(bl_box, br_box, RIGHT);
#endif

}

template< typename node_dtype, typename dimen_t>
Graph_Matrix<node_dtype, dimen_t>::~Graph_Matrix(){
	while( this->_total_y_abs != 0 ){  //excluding the origin node, which will automatically get destructed, since it's not a pointer
		popRow();   //remove the last row ( = n_row-1)     
	}

	while( this->_total_x_abs != 0 ){  //excluding the origin node, which will automatically get destructed, since it's not a pointer
		popCol();
	}

#if DEBUG_MODE
	using std::cout; using std::endl;
	clog << "Removed all"<< endl;
#endif
}

// CONSTRUCTORS AND DESTRUCTORS	//
