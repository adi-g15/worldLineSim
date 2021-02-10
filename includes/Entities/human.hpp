#pragma once
// @future - For a small improvement, have a static member (like 3D bitset representing the areas found by humanity)

// As of now, human justs keeps moving randomly

#include "entity.hpp"

class Human : public Entity {
	Entity_Point curr_pos;
	World* parent_world;
	bool should_wander{ false };
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

	explicit Human(World_Ptr const world);
	//Human(World_Ptr const world, Human const* parter) : Entity();	// the human will be together with the partner, or at least distance
};
