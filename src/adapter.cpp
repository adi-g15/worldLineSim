// there was a circular dependency b/w display.hpp and NodeAdapter.hpp, that's why this has been segregated

#include <mutex>    // for std::call_once
#include "logger.hpp"

#include "adapter.hpp"
#include "display/display.hpp"
#include "world_node.hpp"
#include "world.hpp"

#include <Entities/rock.hpp>
#include <Entities/human.hpp>
#include <Entities/snake.hpp>

using namespace nanogui;

NodeAdapter::NodeAdapter(DisplayScreen dispScr, World_Node_Ptr world_node) :
    dispMngr(dispScr),
    node(world_node),
    Window(dispScr->multiverse_window, "World #.." + std::to_string(world_node->world_id() % 512))
{
    if (!node)    throw std::runtime_error("A non-null world node is required for adapter");

    this->set_layout(new GroupLayout());

    auto data_area = new Widget(this);
    auto layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 15, 2);
    layout->set_spacing(0,10);
    layout->set_col_alignment({ Alignment::Maximum, Alignment::Fill });
    data_area->set_layout(layout);

    if (node->parent_node != nullptr) {
        new Label(data_area, "Diverged from: ");
        new TextBox(data_area, std::string("#") + std::to_string(node->parent_node->world_id() % 512));
    }

    new Label(data_area, "Dimensions: ");
    this->dimension = new Widget(data_area);
    dimension->set_layout(new GridLayout(Orientation::Horizontal, 3));
    new IntBox<statics::dimen_t>(dimension);
    new IntBox<statics::dimen_t>(dimension);
    new IntBox<statics::dimen_t>(dimension);

    new Label(data_area, "Time: ");
    this->time = new IntBox<_timePoint>(data_area);

    new Label(data_area, "Entities: ");
    this->num_entity = new IntBox<int>(data_area);

    auto button_group = new Widget(this);
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
    (new Button(diverge_popup, "With current state", FA_AIR_FRESHENER))->set_callback([&]() mutable {
        this->diverge_without_change();
        });

    auto time_travel_btn = new PopupButton(diverge_popup, "TimeTravel", FA_HOURGLASS);
    time_travel_btn->set_callback([&]() mutable {
        this->diverge_with_time_travel();
        });
    time_travel_btn->set_tooltip("In this you can edit the current world's state !!"
        " You be the God (jyada khush mat hona :D)");

    this->time_travel_popup = time_travel_btn->popup();
    time_travel_popup->set_layout(new GroupLayout());

    auto popup_grid = new Widget(time_travel_popup);
    popup_grid->set_layout(new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 4, 2));

    new Label(popup_grid, "Dimension : ");
    this->new_dimension = new IntBox<int>(popup_grid);
    new_dimension->set_editable(false);

    new Label(popup_grid, "Time : ");
    this->time_travel_time = new IntBox<_timePoint>(popup_grid);
    this->time_travel_time->set_editable(true);

    this->time_travel_entity_panel = new VScrollPanel(time_travel_popup);
    this->time_travel_entity_group = new Widget(time_travel_entity_panel);
    this->time_travel_create_btn = new Button(time_travel_entity_group, "Create !");

    auto pause_btn = new Button(this, "Pause");
    pause_btn->set_background_color({ 255,10,10,50 });

    auto remove_btn = new Button(this, "Remove", FA_STOP);
    remove_btn->set_callback([this]() mutable {
        this->remove();
        });

    pause_btn->set_callback([button_group, pause_btn, this]() mutable {
        this->pause();
        button_group->set_visible(true);
        pause_btn->set_visible(false);
        });

    resume_btn->set_callback([button_group, pause_btn, this]() mutable {
        this->resume();
        button_group->set_visible(false);
        pause_btn->set_visible(true);
        });

    this->center();
    this->update();
}

void NodeAdapter::resume()
{
    LOGGER::log_imp(node->world_id(), Event::World_Resumed);
    this->node->resume_simulation();
}

void NodeAdapter::pause()
{
    LOGGER::log_imp(node->world_id(), Event::World_Paused);
    this->node->pause_simulation();
}

void NodeAdapter::remove()
{  // since it's stopped, so no further calls to this->update() should take place
    this->node->end_simulation();
    this->set_enabled(false);
    this->dispMngr->multiverse_window->remove_child(this);
}

void NodeAdapter::update(){	// updates the content on the window, with updated content from the world_naode that is linked
    this->num_entity->set_value( static_cast<int>( this->node->world.entities.size() ));
    this->time->set_value(this->node->current_time);
    const auto& dimensions = this->dimension->children();
    const auto& curr_dimen = this->node->get_exact_dimen();

    static_cast<IntBox<statics::dimen_t>*>(dimensions[0])->set_value( curr_dimen.mX );
    static_cast<IntBox<statics::dimen_t>*>(dimensions[1])->set_value( curr_dimen.mY );
    static_cast<IntBox<statics::dimen_t>*>(dimensions[2])->set_value( curr_dimen.mZ );
}

void NodeAdapter::update_screen()
{
    this->dispMngr->multiverse_window->center();
    this->dispMngr->redraw();
    this->dispMngr->perform_layout();
}

void NodeAdapter::diverge_without_change()
{
    this->node->diverge_without_change();
    this->update_screen();
}

void NodeAdapter::diverge_with_time_travel()
{
    State editable_state = this->node->return_state();

    std::clog << "Diverging with time travel clicked" << std::endl;

    this->new_dimension->set_value(editable_state.universe_order);

    this->time_travel_time->set_value(editable_state.state_time);
    this->time_travel_time->set_max_value(this->node->current_time);

    this->time_travel_time->set_spinnable(true);
    this->time_travel_time->set_editable(true);
    this->time_travel_time->set_units("u");

    this->time_travel_popup->set_width(this->width());

    this->time_travel_entity_group->set_layout(new GridLayout());

    if (this->time_travel_entity_widgets.size() > editable_state.entity_states.size()) {
        auto i = editable_state.entity_states.size();

        for (; i < time_travel_entity_widgets.size(); ++i)
        {
            this->parent()->remove_child(time_travel_entity_widgets[i][0]);
            this->parent()->remove_child(time_travel_entity_widgets[i][1]);
            //this->parent()->remove_child(time_travel_entity_widgets[i][2]);
        }

        time_travel_entity_widgets.resize(editable_state.entity_states.size());
        time_travel_entity_widgets.shrink_to_fit();
    } else if(time_travel_entity_widgets.size() < editable_state.entity_states.size()) {
        auto i = time_travel_entity_widgets.size();
        
        time_travel_entity_widgets.resize(editable_state.entity_states.size());

        for (; i < editable_state.entity_states.size(); i++)
        {
            const auto& entity_state = editable_state.entity_states[i];

            time_travel_entity_widgets[i][0] = new Label(time_travel_entity_group, 
                Entity_Type_Map[static_cast<int>(entity_state->entity_type)]
            );

            if ((entity_state->entity_type == Entity_Types::HUMAN) || (entity_state->entity_type == Entity_Types::SNAKE) || (entity_state->entity_type == Entity_Types::ROCK)) {
                time_travel_entity_widgets[i][1] = new Widget(time_travel_entity_group);
                time_travel_entity_widgets[i][1]->set_layout(new GridLayout(Orientation::Horizontal, 3));

                switch (entity_state->entity_type)
                {
                case Entity_Types::HUMAN: {
                    const auto& state = static_cast<const HumanState*>(entity_state);

                    (time_travel_entity_widgets[i][1]->add<IntBox<dimen_t>>(state->location.mX))->set_editable(true);
                    (time_travel_entity_widgets[i][1]->add<IntBox<dimen_t>>(state->location.mY))->set_editable(true);
                    (time_travel_entity_widgets[i][1]->add<IntBox<dimen_t>>(state->location.mZ))->set_editable(true);

                    break;
                }
                case Entity_Types::SNAKE: {
                    const auto& state = static_cast<const SnakeState*>(entity_state);

                    (time_travel_entity_widgets[i][1]->add<IntBox<dimen_t>>(state->location.mX))->set_editable(true);
                    (time_travel_entity_widgets[i][1]->add<IntBox<dimen_t>>(state->location.mY))->set_editable(true);
                    (time_travel_entity_widgets[i][1]->add<IntBox<dimen_t>>(state->location.mZ))->set_editable(true);

                    break;
                }
                case Entity_Types::ROCK: {
                    const auto& state = static_cast<const RockState*>(entity_state);

                    (time_travel_entity_widgets[i][1]->add<IntBox<dimen_t>>(state->location.mX))->set_editable(true);
                    (time_travel_entity_widgets[i][1]->add<IntBox<dimen_t>>(state->location.mY))->set_editable(true);
                    (time_travel_entity_widgets[i][1]->add<IntBox<dimen_t>>(state->location.mZ))->set_editable(true);

                    break;
                }
                }
            }
            else
            {
                time_travel_entity_widgets[i][1] = new Label(time_travel_entity_group, "No Location");
            }
        }
    }

    for (auto i=0; i < editable_state.entity_states.size(); ++i)
    {
        auto& entity_state = editable_state.entity_states[i];

        // the entities that definitely have a position
            switch (entity_state->entity_type)
            {
            case Entity_Types::HUMAN: {
                const auto& state = static_cast<const HumanState*>(entity_state);

                static_cast<IntBox<dimen_t>*>(time_travel_entity_widgets[i][1]->child_at(0))->set_value(state->location.mX);
                static_cast<IntBox<dimen_t>*>(time_travel_entity_widgets[i][1]->child_at(1))->set_value(state->location.mY);
                static_cast<IntBox<dimen_t>*>(time_travel_entity_widgets[i][1]->child_at(2))->set_value(state->location.mZ);

                break;
            }
            case Entity_Types::SNAKE: {
                const auto& state = static_cast<const SnakeState*>(entity_state);

                static_cast<IntBox<dimen_t>*>(time_travel_entity_widgets[i][1]->child_at(0))->set_value(state->location.mX);
                static_cast<IntBox<dimen_t>*>(time_travel_entity_widgets[i][1]->child_at(1))->set_value(state->location.mY);
                static_cast<IntBox<dimen_t>*>(time_travel_entity_widgets[i][1]->child_at(2))->set_value(state->location.mZ);

                break;
            }
            case Entity_Types::ROCK: {
                const auto& state = static_cast<const RockState*>(entity_state);

                static_cast<IntBox<dimen_t>*>(time_travel_entity_widgets[i][1]->child_at(0))->set_value(state->location.mX);
                static_cast<IntBox<dimen_t>*>(time_travel_entity_widgets[i][1]->child_at(1))->set_value(state->location.mY);
                static_cast<IntBox<dimen_t>*>(time_travel_entity_widgets[i][1]->child_at(2))->set_value(state->location.mZ);

                break;
            }
            default: break;
        }
    }


    time_travel_create_btn->set_callback([this, editable_state]() mutable {
        editable_state.state_time = time_travel_time->value();
        editable_state.universe_order = new_dimension->value();

        this->node->diverge_with_state(editable_state);
        this->update_screen();
        });

    this->update_screen();
}
