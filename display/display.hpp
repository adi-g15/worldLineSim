#pragma once

#include "adapter.hpp"
#include <chrono>

#include <string>
#include <map>

#include <nanogui/nanogui.h>

class Verse;	// forward-decl

class Display: public nanogui::Screen {
	bool shouldStop{false};	// apart from this the shouldWindowClose() will be considered too
	Verse* parent_verse;
	std::chrono::high_resolution_clock::time_point loggingStart;
	const std::array<const std::string_view, 3> commands{ "help", "pause", "test" };
	std::map<std::string, std::function<void(void)>> shortcut_map;
public:

	nanogui::Window* help_window;
	nanogui::Window* multiverse_window;
	nanogui::Window* legend_window;

	void start_input_daemon();
	void startDisplay();
	void displayCurrentState() const;
	void showExiting();
	void stopDisplay();

	// not using node_adapter currently, not much deep info is needed
	std::shared_ptr<DisplayAdapter> newNodeAdapter(World_Node* node);


	void updateScreen();


	bool paused{ false };	// @deprecated
	void resumeRendering();	// @deprecated
	void pauseRendering();	// @deprecated


	void showExit();
	Display(Verse*);
	~Display();
};
