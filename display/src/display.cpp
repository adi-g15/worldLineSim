#include "../display.hpp"

#include "adapter.hpp"
#include "nanogui/widget.h"
#include "verse.hpp"
#include "world_tree.hpp"

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

using namespace nanogui;

using std::chrono::high_resolution_clock;
void Display::start_input_daemon() {
    std::thread([&]() mutable {
        thread_local std::string entered_command;
        do {
            // std::future<void> cmd_future = std::async(std::launch::async,
            // [&]() mutable { std::getline(std::cin, entered_command); });
            // cmd_future.wait();
            std::getline(std::cin, entered_command);

            std::clog << "\n\n\nGot command: \"" << entered_command << "\"\n\n"
                      << std::endl;
        } while (this->enabled());
    }).detach();
}

void Display::startDisplay() {
    this->start_input_daemon();

    multiverse_window->set_position(
        {0 + 5, this->height() / 10 + 5}); // it is (y,x)
    multiverse_window->set_layout(new GridLayout());
    multiverse_window->set_height(static_cast<int>(0.90f * this->height()) -
                                  10);
    multiverse_window->set_width(static_cast<int>(0.80f * this->width()) - 5);

    legend_window->set_layout(new GroupLayout());
    legend_window->set_position({
        static_cast<int>(0.80f * this->width()) - 5 + 5 +
            5, // 80% if width +- margins
        multiverse_window->position().y(),
    });
    legend_window->set_height(static_cast<int>(0.90f * this->height()) - 10);
    legend_window->set_width(static_cast<int>(0.20f * this->width()) - 5);

    const std::string message1 = "All worlds continue on diff. threads, w/o "
                                 "blocking the display, or the verse";
    const std::string message2 = "After chosing a particular world";

    auto legend_width = legend_window->width();
    { // this emulates `wrapping` the message to correct width in legend_window
        int i = 0;
        while (i < message1.size()) {
            new Label(legend_window,
                      message1.substr(i, (legend_width - 1) / 4));
            i += (legend_width - 1) / 4;
        }
        new Label(legend_window, "");
        i = 0;
        while (i < message2.size()) {
            new Label(legend_window,
                      message2.substr(i, (legend_width - 1) / 4));
            i += (legend_width - 1) / 4;
        }
        new Label(legend_window, "");
    }

    auto command_area = ref<Widget>(new Widget(legend_window));
    command_area->set_layout(new GroupLayout());

    new Label(legend_window, "Namaste/New WorldTree");
    ref<Button> newWorldTree = new Button(legend_window, "New", FA_ATLAS);
    newWorldTree->set_callback(
        [&]() mutable { this->parent_verse->add_world_tree(); });
    // newWorldTree->set_background_color({5,5,5,255});
    newWorldTree->set_width((8 * legend_width) / 10);
    /*
    -   N - Namaste World(New)");
    -   P - Pause");
    -   R - Resume");
    -   T - Time Travel !!");
    -   L - Logs (of World)");
    -   V - Logs (of Verse)"
    */

    this->set_visible(true);
    this->perform_layout();

    std::thread([&, legend_width, newWorldTree]() mutable {
        while (this->enabled()) {
            header->set_height(this->height() / 10); // 10% height
            header->set_width(this->width());

            multiverse_window->set_position(
                {0 + 5, this->height() / 10 + 5}); // it is (y,x)
            multiverse_window->set_layout(new GridLayout());
            multiverse_window->set_height(
                static_cast<int>(0.90f * this->height()) - 10);
            multiverse_window->set_width(
                static_cast<int>(0.80f * this->width()) - 5);

            legend_window->set_position({
                static_cast<int>(0.80f * this->width()) - 5 + 5 +
                    5, // 80% if width +- margins
                multiverse_window->position().y(),
            });
            legend_window->set_height(static_cast<int>(0.90f * this->height()) -
                                      10);
            legend_window->set_width(static_cast<int>(0.20f * this->width()) -
                                     5);

            legend_width = legend_window->width();
            newWorldTree->set_width((8 * legend_width) / 10);

            std::this_thread::sleep_for(
                std::chrono::milliseconds(1000 / 40)); // 40Hz
        }
    }).detach();

    // @future - I am basically using multiple threads for things that could
    // have been done on single thread, like this mainloop and the above
    // updation logic, in future change to glfw or similar for more control
    // (likely won't do so though, maybe doing some other project)
    nanogui::mainloop(1000 / 60.0f); // Refresh every 1000/60 seconds, ie. 60Hz
}

ref<NodeAdapter> Display::add_node_adapter(World_Node *node) {
    auto adapter = new NodeAdapter(this, node);

    // this->multiverse_window->request_focus();
    multiverse_window->center();
    this->redraw();
    this->perform_layout();

    return adapter;
}

void Display::showExiting() {
    // clear();
    // clearAll();

    // mvwaddstr(stdscr, this->_terminal_y/2, (this->_terminal_x - 15)/2, "About
    // to Exit !");
    std::clog << "Exiting...";
}

void Display::stopDisplay() { std::clog << "Exiting NOW!"; }

Display::Display(Verse *parent)
    : // title("WorldLine Simulator v0.271", "Created by Aditya Gupta and
      // Steins; Gate"),
      parent_verse(parent),
      Screen({800, 1000}, "WorldLine Simulator v0.271" /*, false, true */) {
    this->inc_ref();

    this->shortcut_map = {{":help",
                           [&]() mutable {
                               this->help_window->set_visible(true);
                               this->legend_window->set_visible(false);
                               this->multiverse_window->set_visible(false);
                           }},
                          {":about",
                           [&]() mutable {
                               // @future - No plan to add this now
                           }},
                          {":web",
                           [&]() mutable {
                               // nanogui::ImageView*
                           }},
                          {":pause", [&]() mutable {

                           }}};

    //#ifdef _WIN32
    //	MessageBoxA(nullptr, "Hi message", NULL, MB_ICONERROR | MB_OK);
    //	return;
    //#endif // _WIN32

    Color background_col = {20, 20, 23, 255};
    this->set_background(background_col);

    header = new TextBox(this, "WorldLine Simulation v0.271");
    header->set_editable(false);

    // these are needed to be initialised before the world tree starts creating
    // new disp
    multiverse_window = new Window(this, "MultiVerse");

    legend_window = new Window(this, "Legend");
}

Display::~Display() {
    this->dec_ref();

    nanogui::shutdown();
}
