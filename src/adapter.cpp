// there was a circular dependency b/w display.hpp and NodeAdapter.hpp, that's why this has been segregated

#include <mutex>    // for std::call_once
#include "logger.hpp"

#include "adapter.hpp"
#include "display/display.hpp"
#include "world_node.hpp"
#include "world.hpp"

using namespace nanogui;

NodeAdapter::NodeAdapter(DisplayScreen dispScr, World_Node_Ptr world_node) :
    node(world_node),
    Window(dispScr->multiverse_window, "World #.." + std::to_string(world_node->_id%512))
{
    if (!node)    throw std::runtime_error("A non-null world node is required for adapter");

    this->set_layout(new GroupLayout());

    auto data_area = new Widget(this);
    auto layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 15, 2);
    layout->set_spacing(0,10);
    layout->set_col_alignment({ Alignment::Maximum, Alignment::Fill });
    data_area->set_layout(layout);

    new Label(data_area, "Dimensions: ");
    this->dimension = new Widget(data_area);
    dimension->set_layout(new GridLayout(Orientation::Horizontal, 3));
    new IntBox<statics::dimen_t>(dimension);
    new IntBox<statics::dimen_t>(dimension);
    new IntBox<statics::dimen_t>(dimension);

    new Label(data_area, "Time: ");
    this->time = new FloatBox<_timePoint>(data_area);

    new Label(data_area, "Entities: ");
    this->num_entity = new IntBox<int>(data_area);

    ref<Widget> button_group = new Widget(this);
    button_group->set_layout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 6));

    auto resume_btn = new Button(button_group, "Resume", FA_PLAY);
    auto diverge_btn = new PopupButton(button_group, "Diverge", FA_CODE_BRANCH);
    //auto diverge_btn = new ToolButton(button_group, FA_CODE_BRANCH, "Diverge");
    diverge_btn->set_tooltip(
        "You can chose to alter the world's state"
        " by going back in time, hence changing the FUTURE :D"
    );

    auto diverge_popup = diverge_btn->popup();
    diverge_popup->set_layout(new GroupLayout());
    new Button(diverge_popup, "With current state", FA_AIR_FRESHENER);
    new Button(diverge_popup, "TimeTravel", FA_HOURGLASS);

    auto pause_btn = new Button(this, "Pause");
    pause_btn->set_background_color({ 255,10,10,50 });

    pause_btn->set_callback([button_group, pause_btn, this]() mutable {
        LOGGER::log_it(node->_id, Event::World_Paused);
        button_group->set_visible(true);
        pause_btn->set_visible(false);
        });

    resume_btn->set_callback([button_group, pause_btn, this]() mutable {
        LOGGER::log_it(node->_id, Event::World_Resume);
        button_group->set_visible(false);
        pause_btn->set_visible(true);
        });

    (new Button(this, "Trigger Update"))->set_callback([&]() mutable {
        this->update();
        });

    this->entity_group = new Widget(this);
    this->entity_group->set_layout(new GridLayout(Orientation::Horizontal, 2, Alignment::Minimum));

    this->update();
}

void NodeAdapter::update(){	// updates the content on the window, with updated content from the world_naode that is linked
    this->num_entity->set_value(this->node->world->entities.size());
    this->time->set_value(this->node->current_time);
    const auto& dimensions = this->dimension->children();
    const auto& curr_dimen = this->node->get_exact_dimen();

    static_cast<IntBox<statics::dimen_t>*>(dimensions[0])->set_value( curr_dimen.mX );
    static_cast<IntBox<statics::dimen_t>*>(dimensions[1])->set_value( curr_dimen.mY );
    static_cast<IntBox<statics::dimen_t>*>(dimensions[2])->set_value( curr_dimen.mZ );
}
