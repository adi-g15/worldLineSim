#pragma once

#include <vector>
#include <utility>
#include <stdexcept>

#include "forward_decl.hpp"
#include "util/coord.hpp"
#include "display/curses_subwin.hpp"

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

	node_adapter(DispMngr dispMngr, World_Node_Ptr world_node, int height, int width,int y_corner, int x_corner);

public:
	void update();

	node_adapter() = delete;
	~node_adapter(){}

	friend class Display;
};
