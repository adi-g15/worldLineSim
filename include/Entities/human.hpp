#pragma once
// @future - For an interesting improvement, have a static member (like 3D
// bitset representing the areas found by humanity) As of now, human just keep
// moving randomly

#include "db/database.hpp"
#include "entity.hpp"

class Human;

struct HumanState : public EntityState {
    Gender gender;
    coord location;
    id_type old_id;

    HumanState(const Human *);
};

class Human : public Entity {
  protected:
    Entity_Point curr_pos;
    World *parent_world;
    bool should_wander{false};
    bool has_been_paused{true};
    const float TIME_DIFF_PER_MOVE{
        1.0f}; // ie. one `time unit` between each move

    void _Action1() override {}
    void _Action2() override {}

  public:
    typedef World *World_Ptr;
    Gender gender;
    std::string mName;

    // static std::vector<std::vector<std::bitset>> > humanities_exploration;
    // static GraphMatrix3D<bool> humanities_exploration;

    std::optional<Entity_Point> getPrimaryPos() const override;
    void simulateExistence() override;
    void pauseExistence() override;

    HumanState *_get_current_state() const override;

    Human(World_Ptr const world, const std::string &name, Gender);
    Human(World_Ptr const world, Gender);
    Human(World_Ptr const world, const HumanState &prev_state);
    ~Human();
    // Human(World_Ptr const world, Human const* parter) : Entity();	// the
    // human will be together with the partner, or at least distance

    friend struct HumanState;
};
