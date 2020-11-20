#pragma once

#include <vector>
#include <utility>
#include <random>	// only for ids, remove this later
#include <stdexcept>

#include "curses.h"
#include "declarations.hpp"
#include "util/math.hpp"

// class disp_statics{
// 	private:
// 			// this should be in sync with the_occupy_table (since the positioning isn't random now, and is determined by that only)
// 		static int next_index;	// expecting by default it will be 0

// 	friend class node_adapter;
// };

class node_adapter{
	typedef util::_coord<int> coordinate;	// on a screen so won't need anything bigger than an int
// private:
public:
	WINDOW* window{nullptr};

	// the further data can be replaced by having a pointer t the wrld_node instead of all this individually
	size_t node_id;
	int index_num;	// index number

	// int x_dimen;
	// int y_dimen;
	std::vector< coordinate > vector_of_pos;	// vector of 2D `coordinates`

	// bool world_stale;	// future - Remove nodes from display that are no more working

public:
	const char* sample_msg(){
		return "3rd hi from node";
	}

	node_adapter() = delete;
	node_adapter(WINDOW* parent_window, int par_y_corner, int par_x_corner, std::vector<std::vector<bool>>& the_occupy_table, int index ){	// the occupy table keeps record of what regions are occupied (also called, occupancy_table)
		if(the_occupy_table.size() == 0){
			throw std::logic_error("The OCCUPANCY_TABLE should have at least 1 row");	// since when deciding the coords for node_adapter, we access .at(0)
		}

		this->index_num = index;

		this->node_id = static_cast<size_t>(std::random_device{}());

		int y_length = 10;	// these should be same for all, as of the current situation
							// @future - Try to make these adapt to, for eg. more snakes, so ONLY the next code for determining the position should need change, after the y_length, and x_length aren't same for all node_adapters anymore
		int x_length = 17;

		int start_pos_x, start_pos_y;	// @todo - Decide these

		// if( disp_statics::next_index < 8 ){	// for now only 8 boxes will be there
		if( this->index_num < 8 ){	// for now only 8 boxes will be there
			// @note @adig15 - I am leaving these additional variables, so as for a `faster` understanding for the future me, and lesser thinking for me `now`, as i don't actually want to go deep in this
			// @note @adig15 - The occupy_table is actually not needed for this simple case, but will be needed if i want (slightly(ie. it must be clear which one was created first)) random positioning of the node_adapters
			int bigBox_coord_x;
			int bigBox_coord_y;	// the big box's corner coord

			bigBox_coord_y = ((the_occupy_table.size()+1)/2) * (this->index_num / 4);	// first row or second row
			bigBox_coord_x = ((the_occupy_table.at(0).size()+1)/4) * ( this->index_num % 4 );

			if( this->index_num %2 == 0 ){
				start_pos_y = bigBox_coord_y;
			}else{
				start_pos_y = bigBox_coord_y + 2;	// 2 lines below
			}
// 91, 12
				// here what we want is to `horizonatlly center` the node_adapter inside this big_box
			start_pos_x = bigBox_coord_x + ( (the_occupy_table.at(0).size()+1)/4 - x_length ) / 2;
		}else{
			// @todo @future - This case needs to be handled, and likely requires a queue, ie. the first of the 8 nodes, will need to be removed first... ie. remove the top left node in the main_area, and shift all other 7 nodes, and at the bottom right add a new node_adapter
		}

			// the if condition in this for loop is 'stop at whichever is lesser,  start_pos_y + y+length, or we have reached the limit itself (that is number of rows in main_area (though this shouldn't be the case, we expect enough spaces for all 8))'
		for (int i = start_pos_y; i < (start_pos_y + y_length) && i < the_occupy_table.size() ; i++)
		{
			for (int j = start_pos_x; j < (start_pos_x + x_length) && j < the_occupy_table.at(i).size() ; j++)
			{
				the_occupy_table[i][j] = true;	// set the bit at i,j in occupancy table
			}
		}

		this->window = subwin(parent_window, y_length, x_length, start_pos_y + par_y_corner + 1, start_pos_x + par_x_corner + 1);
		box(this->window, ACS_VLINE, '-');
		mvwaddstr(this->window, 1, std::max((x_length - 2 - util::num_digits(this->node_id))/2, 1), std::to_string(this->node_id).data());

		mvwhline(this->window, 2, 1, ACS_HLINE,x_length-2);
		mvwprintw(this->window, 3, 1, "Dimen - (%d, %d)", 10, 10);

		for (int i = 0; i < 4; i++)
		{
			mvwprintw(this->window, 4+i, 1, "E%d - (%d, %d), %d", i+1, std::random_device{}()%10,  std::random_device{}()%10, std::random_device{}()%100);
		}

		wrefresh(this->window);

	}
	~node_adapter(){
		if( this->window != nullptr )
			delwin(this->window);
	}
};
