#pragma once

#include <list>

#include "id_creator.hpp"
#include "state.hpp"
#include "world_tree.hpp"
#include "world.hpp"
#include "adapter.hpp"

typedef int32_t dimen_t;

class World_Node{
public:
	typedef World_Node* World_Node_Ptr;
	typedef nanogui::ref<NodeAdapter> Adapter_Ptr;

	Adapter_Ptr adapter;
	World_Tree* tree;

	const id_type world_id() const noexcept;

	void diverge_without_change();	// creates a new node, from current time itself, without change
	void diverge_with_state(const State& custom_state);

	const World& get_world() const;
	const _timePoint getCurrentTime() const;

	const dimen_t get_world_order() const;
	const coord& get_exact_dimen() const;

	World_Node( World_Tree* tree);
	World_Node( World_Node* parent_node, _timePoint t );
	World_Node( World_Node* parent_node, const State& prev_state );

	~World_Node();

private:
	_timePoint current_time;

	World world;

	std::vector<State> states;    // holds all states since it's formation

	const _timePoint start_time;  // time this world_node was created (relative to it's verse)

	World_Node_Ptr parent_node{ nullptr }; // this being nullptr, means that `this` node is the root itself
	std::list<World_Node_Ptr> diverged_child;	// child node
		// it logically is still a tree

	void capture_state();

	// this returns `current/latest state`
	const State& return_state();

	// this returns `past state` time t_back back into the past
	const State& return_state(_timePoint t_back);

	void handle_pause(State that_state);

	void start_state_management();
	void resume_simulation();
	void pause_simulation();
	void end_simulation();	// IRRECOVERABLE

	friend class World_Tree;
	friend class NodeAdapter;
};
