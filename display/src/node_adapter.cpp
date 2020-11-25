// there was a circular dependency b/w display.hpp and node_adapter.hpp, that's why this has been segregated

#include <mutex>    // for std::call_once

#include "display/node_adapter.hpp"
#include "display/display.hpp"
#include "display/curses_subwin.hpp"
#include "world_node.hpp"

node_adapter::node_adapter(DispMngr dispMngr, World_Node_Ptr world_node, int height, int width,int y_corner, int x_corner)
    :  node(world_node),
    dispMngr(dispMngr),
    window(dispMngr->main_area, height, width, y_corner, x_corner)
{
    if( ! node )    throw std::invalid_argument("A non-null world pointer is required")

    this->node_id = node->getId();

    window.box(ACS_VLINE, '-');

    window.moveCursor(1, 1);
    window.addstr(std::to_string(this->node_id).data(), position::MIDDLE);
    window.hline();

    window.newline();
    window.printf("Dimen - (%, %)", this->node->get_world_dimen(), this->node->get_world_dimen());
    // window.printf("Dimen - (, )");

    for (int i = 0; i < 4; i++)
    {
        window.newline();
        // window.printf("E - (, ), ");	// snake number/id, head_coord.x, head_coord.y, points of snake
        window.printf("E% - (%, %), %", i+1);	// snake number/id, head_coord.x, head_coord.y, points of snake
    }

    window.refresh();
    wrefresh(stdscr);
}

void node_adapter::update(){	// updates the content on the window, with updated content from the world_naode that is linked
    if( this->dispMngr->paused )	return;

    // @todo - Handle the display here

    std::call_once( this->dispMngr->disp_once_flag, &Display::updateScreen, this->dispMngr );
}
