#pragma once

#include "adapter.hpp"
#include <chrono>

#include <string>
#include <map>

#include <nanogui/nanogui.h>

class Verse;	// forward-decl

class Display: public nanogui::Screen {
public:
	using Window_Ptr = nanogui::Window*;
	using Adapter_Ptr = nanogui::ref<NodeAdapter>;

	Verse* parent_verse;

	void start_input_daemon();
	void startDisplay();
	void showExiting();
	void stopDisplay();

	Adapter_Ptr add_node_adapter(World_Node* node);

	void showExit();
	Display(Verse*);
	~Display();

private:
	bool shouldStop{ false };	// apart from this the shouldWindowClose() will be considered too
	const std::array<const std::string, 3> commands{ "help", "pause", "test" };
	std::map<std::string, std::function<void(void)>> shortcut_map;

	nanogui::ref<nanogui::TextBox> header;
	Window_Ptr help_window;
	Window_Ptr multiverse_window;
	Window_Ptr legend_window;

	friend class NodeAdapter;
};
