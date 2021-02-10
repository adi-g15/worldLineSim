#pragma once

#include "forward_decl.hpp"
#include "declarations.hpp"
#include "entity.hpp"
#include "graphMat/direction.hpp"
#include "graphMat/3d_graph_box.hpp"

#include <vector>
#include <list>
#include <memory>
#include <cstdint>
#include <condition_variable>

class Snake;    // forward-declaration for use in SnakeBody

// @note - `directionalPath` is alias for a `std::vector`, while `SnakeBody` is an alias for a `std::list`
struct SnakeBody {
   // @order -> body.front() is the first direction from the body_head
    std::list<Direction> body; //list because, popping at back and pushing in front both required quite regulRLY, SO CANT use vector, shift is O(N)
    Snake* const snake_ptr;

    auto pop_back() { body.pop_back(); }
    void grow(Direction move_dir);
    void move(Direction move_dir);  // push front opposite direction to this

    auto length() const { return body.size(); }
    void removeAndClearBody();
    const auto begin() const { return body.begin(); }
    const auto end() const { return body.end(); }

    SnakeBody(Snake*);
};

struct SnakeState : public EntityState {
    const int length;
    const coord curr_location;

    SnakeState(const Snake* snake);
};

class Snake: public Entity{
    // typedef std::shared_ptr<World> World_Ptr;
    typedef World* World_Ptr;
    typedef int32_t dimen_t;
    typedef std::make_unsigned_t<dimen_t> udimen_t;

    mutable struct {
            // size 10 since 10 directions
        std::array<int, 10> bucket;  // a temporary bucket for use in Snake::hasRoundTrips
        Direction last_dir; // used in move_Forward()
    }__temp;

public:
    const World_Ptr parent_world;

    // std::vector< coord_type > curr_path;
    directionalPath curr_Path;
    //udimen_t length;   //starts with _world.init_length

    void _Action1() override;   //calls moveForward()
    void _Action2() override;   //calls eatFood()
    std::optional<Entity_Point> getPrimaryPos() const override;

    bool isSnakeBodyOK() const;

    bool isSimulating{false};
    std::condition_variable sim_convar;
    void simulateExistence() override;  // calls moveForward, and other logic, for it to exist `independently (other than the needed interactions b/w entities)` from other entities
    void pauseExistence();  // just `pauses` any further movement, it still will be on board

    EntityState _get_current_state() const override;

    int getUniqProp() const;
    int getLength() const;

    const Entity_Point& getHead() const;
    const Entity_Point& getTail() const;

    explicit Snake(const World_Ptr);
    Snake(const World_Ptr, uint16_t);
    ~Snake();
private:

    SnakeBody body;

    Entity_Point head;
    Entity_Point tail;

    // @note - eating doesn't do ANY movemoent
    bool eatFood(); //returns true if it can eat, else false and no change
    bool hasRoundTrips() const;
    bool isPathValid() const;
    void moveForward(); // continue on the acquired path, or if it's not available, find a new one

    // HELPER FuNCTIonS
    void _add_dir_to_coord(coord&, Direction) const;

    friend struct SnakeBody;
    friend struct SnakeState;
};
