#pragma once

#include <vector>
#include <utility>
#include <stdexcept>

#include "curses.h"
#include "util/math.hpp"

#include "declarations.hpp"
#include "display/display.hpp"
#include "world_node.hpp"

// class disp_statics{
// 	private:
// 			// this should be in sync with the_occupy_table (since the positioning isn't random now, and is determined by that only)
// 		static int next_index;	// expecting by default it will be 0

// 	friend class node_adapter;
// };

class node_adapter{
	typedef util::_coord<int> coordinate;	// on a screen so won't need anything bigger than an int
	typedef std::shared_ptr<Display> DispMngr;
	typedef World_Node* World_Node_Ptr;
// private:
public:
	SubWindow window;	// @note @caution -> The node_adapter's SubWindow isn't meant to be passed around ! (If tried to create a shared_ptr from this, will give double free)
	DispMngr dispMngr;	// @todo - Make this a non-modifieable by this class (should always point to same dispMngr)

	// the further data can be replaced by having a pointer t the wrld_node instead of all this individually
	size_t node_id;
	const World_Node_Ptr node;
	int index_num;	// index number

	// int x_dimen;
	// int y_dimen;
	std::vector< coordinate > vector_of_pos;	// vector of 2D `coordinates`

	// bool world_stale;	// future - Remove nodes from display that are no more working

	node_adapter(DispMngr dispMngr, World_Node_Ptr world_node, int height, int width,int y_corner, int x_corner)
	 :  node(world_node),
		dispMngr(dispMngr),
		window(dispMngr->main_area, height, width, y_corner, x_corner)
	{
		this->node_id = node->getId();

		window.box(ACS_VLINE, '-');

		window.moveCursor(1, 1);
		window.addstr(std::to_string(this->node_id).data(), position::MIDDLE);
		window.hline();

		window.newline();
		window.printf("Dimen - (%, %)", this->node->get_world_dimen(), this->node->get_world_dimen());

		for (int i = 0; i < 4; i++)
		{
			window.newline();
			window.printf("E% - (%, %), %", i+1);	// snake number/id, head_coord.x, head_coord.y, points of snake
		}

		window.refresh();
	}

public:
	void update(){	// updates the content on the window, with updated content from the world_naode that is linked
		if( this->dispMngr->paused )	return;

		// @todo - Handle the display here
	}

	node_adapter() = delete;
	~node_adapter(){
		if( this->window != nullptr )
			delwin(this->window);
	}

	friend class Display;
};
