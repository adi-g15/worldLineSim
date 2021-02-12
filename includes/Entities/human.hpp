#pragma once
// @future - For a small improvement, have a static member (like 3D bitset representing the areas found by humanity)

// As of now, human justs keeps moving randomly

#include "entity.hpp"

class Human;

struct HumanState : public EntityState {
	const coord location;

	HumanState(const Human*);
};

class Human : public Entity {
	Entity_Point curr_pos;
	World* parent_world;
	bool should_wander{ false };
	bool has_been_paused{ true };
	const float TIME_DIFF_PER_MOVE{ 1.0f };	// ie. one `time unit` between each move

	void _Action1() override {}
	void _Action2() override {}
public:
	typedef World* World_Ptr;

	//static std::vector<std::vector<std::bitset>> > humanities_exploration;
	//static GraphMatrix3D<bool> humanities_exploration;

	std::optional<Entity_Point> getPrimaryPos() const override;
	void simulateExistence() override;
	void pauseExistence() override;

	HumanState* _get_current_state() const override;

	explicit Human(World_Ptr const world);
	Human(World_Ptr const world, const HumanState& prev_state);
	~Human();
	//Human(World_Ptr const world, Human const* parter) : Entity();	// the human will be together with the partner, or at least distance

	friend struct HumanState;
};
