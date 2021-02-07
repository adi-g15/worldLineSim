#pragma once

#include "adapter.hpp"
#include <chrono>
#include <string_view>

class Verse;	// forward-decl

class Display {
	bool shouldStop{false};	// apart from this the shouldWindowClose() will be considered too
	Verse* parent_verse;
	std::chrono::high_resolution_clock::time_point loggingStart;
	const std::array<const std::string_view, 3> commands{ "help", "pause", "test" };

public:
	void start_input_daemon(){}
	void showInitiating();
	void startDisplay();
	void displayCurrentState() const;
	void showExiting();
	void stopDisplay();

	// not using node_adapter currently, not much deep info is needed
	std::shared_ptr<DisplayAdapter> newNodeAdapter(World_Node* node);


	void helpScreen();
	void updateScreen();


	bool paused{ false };	// @deprecated
	void resumeRendering();	// @deprecated
	void pauseRendering();	// @deprecated


	void showExit();
	Display(Verse*);
};
