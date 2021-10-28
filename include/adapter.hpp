#pragma once

import <memory>;
import <stdexcept>;
import <utility>;
import <vector>;

#include <nanogui/nanogui.h>

#include "declarations.hpp"
#include "forward_decl.hpp"

class NodeAdapter : public nanogui::Window {
  public:
    typedef nanogui::ref<Display> DisplayScreen;
    typedef World_Node *World_Node_Ptr;

    DisplayScreen dispMngr;

    NodeAdapter(DisplayScreen, World_Node_Ptr world_node);
    void resume();
    void pause();
    // similar to stopping it, it will remain paused forever
    void remove(); // removes the adapter from display
    void update();
    void update_screen();

    void diverge_without_change();
    void diverge_with_time_travel();

  private:
    nanogui::IntBox<int> *num_entity, *new_dimension;
    nanogui::IntBox<_timePoint> *time, *time_travel_time;

    // nanogui::Widget* entity_group;
    nanogui::Popup *time_travel_popup;
    nanogui::Widget *dimension;

    nanogui::Button *time_travel_create_btn;
    nanogui::VScrollPanel *time_travel_entity_panel;
    nanogui::Widget *time_travel_entity_group;
    std::vector<std::array<nanogui::ref<nanogui::Widget>, 2>>
        time_travel_entity_widgets;

    World_Node_Ptr node;
};
