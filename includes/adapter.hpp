#pragma once

#include <vector>
#include <utility>
#include <stdexcept>
#include <memory>

#include <nanogui/nanogui.h>

#include "forward_decl.hpp"
#include "declarations.hpp"


class NodeAdapter: public nanogui::Window {
public:
	typedef nanogui::ref<Display> DisplayScreen;
	typedef World_Node* World_Node_Ptr;

	NodeAdapter(DisplayScreen, World_Node_Ptr world_node);
	void disable();	// once disabled, can NOT be enabled again for NOW (since we can't chose which worlds to show and which ones not to show) !
	void update();

private:
	nanogui::IntBox<int>* num_entity;
	nanogui::FloatBox<_timePoint>* time;

	nanogui::Widget* entity_group;
	nanogui::Widget* dimension;

	DisplayScreen dispMngr;
	const World_Node_Ptr node;

};
