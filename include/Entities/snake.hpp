#pragma once

#include "declarations.hpp"
#include "entity.hpp"
#include "graphMat/direction.hpp"
#include "graphMat/graph_box.hpp"

#include <condition_variable>
#include <cstdint>
#include <list>
#include <memory>
#include <vector>

class Snake; // forward-declaration for use in SnakeBody

/**
 *
 * Earlier -
 * After moving we used to add the `opposite of direction moved` to SnakeBody,
 * so, SnakeBody USED TO be FROM head to TAIL, and it seemed more logical to me
 *
 *                      ^
 *                      |
 *                    ->
 *                   ^
 *                   |
 *                   ^
 *                   |
 *                 head
 *                   |
 *                   v
 *          tail <- <-
 *
 * Now -
 * But, now i changed it to be same direction pushed in body, so the snakebody
 * is now FROM tail TO head
 *
 *                      ^
 *                      |
 *                    ->
 *                   ^
 *                   |
 *                   ^
 *                   |
 *                 head
 *                   ^
 *                   |
 *          tail -> ->
 *
 * Notice the end part, now it is FROM tail to head
 *
 */

struct SnakeBody {
    // @order -> body.front() is the first direction from the body_head
    std::list<Direction>
        body; // list because, popping at back and pushing in front both
              // required quite regulRLY, SO CANT use vector, shift is O(N)
    Snake *const snake_ptr;

    auto pop_back() { body.pop_back(); }
    void grow(Direction move_dir);
    void move(Direction move_dir); // push front opposite direction to this

    auto length() const { return body.size(); }
    void removeAndClearBody();
    const auto begin() const { return body.begin(); }
    const auto end() const { return body.end(); }

    SnakeBody(Snake *);
};

struct SnakeState : public EntityState {
    // length - simply body.length() + 1
    const coord location;
    const std::vector<Direction> body;

    SnakeState(const Snake *snake);
};

class Snake : public Entity {
    // typedef std::shared_ptr<World> World_Ptr;
    typedef World *World_Ptr;
    typedef int32_t dimen_t;
    typedef std::make_unsigned_t<dimen_t> udimen_t;

    mutable struct {
        // size 10 since 10 directions
        std::array<int, 10>
            bucket; // a temporary bucket for use in Snake::hasRoundTrips
        Direction last_dir; // used in move_Forward()
    } _temp;

  public:
    const World_Ptr parent_world;

    directionalPath curr_Path;

    void _Action1() override; // calls moveForward()
    void _Action2() override; // calls eatFood()
    std::optional<Entity_Point> getPrimaryPos() const override;

    bool isSnakeBodyOK() const;

    bool should_simulate{false};
    bool is_actively_simulating{false};
    std::condition_variable sim_convar;
    void simulateExistence()
        override; // calls moveForward, and other logic, for it to exist
                  // `independently (other than the needed interactions b/w
                  // entities)` from other entities
    void pauseExistence(); // just `pauses` any further movement, it still will
                           // be on board

    SnakeState *_get_current_state() const override;

    int getUniqProp() const;
    int getLength() const;

    const Entity_Point &getHead() const;
    const Entity_Point &getTail() const;

    explicit Snake(const World_Ptr);
    Snake(const World_Ptr, const SnakeState &initial_state);
    ~Snake();

  private:
    SnakeBody body;

    Entity_Point head;
    Entity_Point tail;

    // @note - eating doesn't do ANY movemoent
    bool eatFood(); // returns true if it can eat, else false and no change
    bool hasRoundTrips() const;
    bool isPathValid() const;
    void moveForward(); // continue on the acquired path, or if it's not
                        // available, find a new one

    // HELPER FuNCTIonS
    void _add_dir_to_coord(coord &, Direction) const;

    friend struct SnakeBody;
    friend struct SnakeState;
};
