#pragma once

#include <vector>
#include <utility>
#include <stdexcept>
#include <memory>

#include "forward_decl.hpp"
#include "declarations.hpp"
#include "display/subwin.hpp"

class DisplayAdapter{
	typedef util::_coord3D<int> coordinate;	// on a screen so won't need anything bigger than an int
	typedef std::shared_ptr<Display> DisplayManager;
	typedef World_Node* World_Node_Ptr;
// private:
public:
	SubWindow window;	// @note @caution -> The DisplayAdapter's SubWindow isn't meant to be passed around ! (If tried to create a shared_ptr from this, will give double free)
	DisplayManager dispMngr;	// @todo - Make this a non-modifieable by this class (should always point to same dispMngr)

	// the further data can be replaced by having a pointer t the wrld_node instead of all this individually
	size_t node_id;
	const World_Node_Ptr node;

	DisplayAdapter(DisplayManager dispMngr, World_Node_Ptr world_node, int height, int width,int y_corner, int x_corner);

public:
	void disable();	// once disabled, can NOT be enabled again for NOW (since we can't chose which worlds to show and which ones not to show) !
	void update();

	DisplayAdapter() = delete;
	~DisplayAdapter(){}

	friend class Display;
};
