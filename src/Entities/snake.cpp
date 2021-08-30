#include <graphMat/iterators.hpp>
#include <util/random.hpp>

#include <map>
#include <thread>
#include <chrono>

#include "Entities/snake.hpp"
#include "world.hpp"
#include "declarations.hpp"

void Snake::_Action1(){
    this->eatFood();    // @note - Ignoring returned boolean from eatFood(), if that's required use eatFood() directly, this method is just for the sake for generalisation using Entity
}

void Snake::_Action2(){
    this->moveForward();
}

std::optional<Entity_Point> Snake::getPrimaryPos() const
{
    return this->head;
}

void Snake::simulateExistence() {
    // @future -> Can add more logic here, when more interaction options between entities are added

    this->should_simulate = true;
    this->is_actively_simulating = true;

    while (this->should_simulate && this->parent_world->is_world_running() ){   //while the parent world continues to exist keep the entity moving
        LOGGER::log_msg("Moving Snake #{}", this->_id);

        // this->moveForward(); // BUG: See issue #5 - https://github.com/adi-g15/worldLineSim/issues/5

        std::this_thread::sleep_for( statics::UNIT_TIME );
    }

    this->should_simulate = false;
    this->is_actively_simulating = false;
}

void Snake::pauseExistence()
{
    if (!this->should_simulate) return;

    this->should_simulate = false;
    while (this->is_actively_simulating == true) {}  // block untill simulateExistence() returns
}

bool Snake::eatFood(){  // @note - The body.grow() needs to be called in caller function
    if (!parent_world->lockFood(this))   return false;   // couldn't eat, food is locked

    this->parent_world->ateFood(this);
    return true;
}

bool Snake::hasRoundTrips() const{
        // since array calls the initializer for its object type, so it is `value initialised IN THIS CASE`, ie 0
    auto& temp_bucket = this->_temp.bucket;
    std::fill(temp_bucket.begin(), temp_bucket.end(), 0); // ensuring all of the array are zero

    for (auto& dir : this->body)
    {
        temp_bucket[static_cast<uint8_t>(dir)]++;
    }

    for (auto i = 0; i < temp_bucket.size(); ++i)
    {
        auto& num_curr = temp_bucket[i];
        auto& num_curr_oposite = temp_bucket[static_cast<uint8_t>(static_cast<Direction>(num_curr))];
        if (num_curr != num_curr_oposite) {
            return false;
        }

        num_curr = num_curr_oposite = 0;
    }

    return true;
}

bool Snake::isPathValid() const{    // checks that `no two adjacent` directions should be opposites
    auto& p = this->body;

    return std::adjacent_find(p.begin(), p.end(), &util::areDirectionsOpposite) == p.end();
}

bool Snake::isSnakeBodyOK() const{
    return isPathValid() && !hasRoundTrips();
}

/**
 * @brief -> Move using path.front()
 * 
 * If path is empty, that means the snake has already mved using the path, and is now JUST on the food, so eat it now
 *
 **/
void Snake::moveForward(){

    if (this->parent_world->world_plot.get_food().box == nullptr)    return;

    this->parent_world->getPathToFood(this->head, this->curr_Path);

    if (this->curr_Path.empty()) {  // reached food's location
        if (this->eatFood())
        {
            body.grow(_temp.last_dir);
            return;
        }
    }

    _temp.last_dir = curr_Path.next_dir();
    body.move(_temp.last_dir);
    curr_Path.pop();
}

void Snake::_add_dir_to_coord(coord& c, Direction dir) const
{
    switch (dir)
    {
    case Direction::UTTAR:
        c += std::array{0, 1, 0};
        break;
    case Direction::PURVA:
        c += std::array{1, 0, 0};
        break;
    case Direction::PASHCHIM:
        c += std::array{-1, 0, 0};
        break;
    case Direction::DAKSHIN:
        c += std::array{0, -1, 0};
        break;
    case Direction::ISHANYA:
        c += std::array{1, 1, 0};
        break;
    case Direction::AGNEYA:
        c += std::array{1, -1, 0};
        break;
    case Direction::NAIRUTYA:
        c += std::array{-1, -1, 0};
        break;
    case Direction::VAYAVYA:
        c += std::array{-1, 1, 0};
        break;
    case Direction::URDHWA:
        c += std::array{0, 0, 1};
        break;
    case Direction::ADHARASTHA:
        c += std::array{0, 0, -1};
        break;
    }
}

SnakeState* Snake::_get_current_state() const
{
    return new SnakeState(this);
}

const Entity_Point& Snake::getHead() const{
    return this->head;
}

const Entity_Point& Snake::getTail() const
{
    return this->tail;
}

int Snake::getUniqProp() const{
    return this->getLength();
}

int Snake::getLength() const
{
    return static_cast<int>( this->body.length() + 1 );
}

/*
@bug/enhancement alert - 
    As per the current code, let's consider a situation,
    Let one snake already on board, eg. the line x=4;

    BUT, what if the snake formed is such that, it is y = 3 (or anything);
    THOUGH, isCellEmpty() will return false, but the actual problem is
    "The next move this new snake moves, will cause the new snake to `collide` with the older one, thereby ending its life in starting itself"
*/
Snake::Snake(const World_Ptr world) :
    Entity(Entity_Types::SNAKE),
    parent_world(world),
    body(this),
    head(nullptr, world->world_plot.getRandomCoord()),
    tail(head)
{
    auto& init_len = world->_init_SnakeLength;
    LOGGER::log_trace("Snake #{} initiated in world #{}", this->_id, world->_id);

    this->head.graph_box = this->parent_world->get_box(head.point_coord);
    assert(head.graph_box != nullptr);

    this->head.graph_box->getDataRef().addEntity(this); // head is a part of the entity

    this->tail = this->head;

    // @note @future - To optimise path finding later, better modify this to only chose dimensions within some distance from 0,0,0

    this->tail.point_coord = this->head.point_coord;
    this->tail.graph_box = this->head.graph_box;
    do {
        if (this->body.length() > 0) LOGGER::log_trace("Clearing Snake: {}", this->_id);

        this->body.removeAndClearBody(); // we are repeating this over and over again, till we have okay body, that's why we are doing this

        auto* prev_box = this->head.graph_box;
        for (auto i = 0; i < init_len - 1; i++) {
            // @future @note @me - The earlier implementation used randomly getting directions, then add_dir_to_coord neighbours

            graphMat::NeighbourIterator<Box> iter(prev_box);
            for (; iter; ++iter)
            {
                // 
                if (!iter->getData().hasEntities()) {
                    prev_box = iter.curr_box;
                    break;
                }
            }
            if (!iter) {
                LOGGER::log_trace("Snake #{} Not enough space could be traversed to allocate snake to that area", this->_id);
            }

            prev_box->getDataRef().addEntity(this);
            this->body.body.push_back(iter._getLastTurnedDirection().value());  // getLastTurnedDirection() MUST have a value

#ifdef HIGH_VERBOSITY
            LOGGER::log_trace("Added unit to snake's body: {}", this->_id);
#endif // HIGH_VERBOSITY

            this->tail.graph_box = prev_box;
            this->tail.point_coord += iter._getIncrementCoords();
        }

    } while ( !isSnakeBodyOK() );
}

Snake::Snake(const World_Ptr parent_world, const SnakeState& initial_state):
    Entity(Entity_Types::SNAKE),
    parent_world(parent_world),
    body(this),
    head(nullptr, initial_state.location),
    tail(head)
{
#ifdef DEBUG
    assert(initial_state.entity_type == Entity_Types::SNAKE);
#endif // DEBUG
    LOGGER::log_trace("Snake #{} initiated in world #{}", this->_id, parent_world->_id);

    // this->body.body.reserve(initial_state.body.size());
    std::copy(initial_state.body.begin(), initial_state.body.end(), std::back_inserter(this->body.body));

    head.graph_box = parent_world->get_box(head.point_coord);
    tail.graph_box = parent_world->get_box(head.point_coord);

    for (auto& body_dir : this->body)
    {
        // updating coords, for now just do add_dir_to_coord
        tail.graph_box = tail.graph_box->get_adj_box(body_dir);
        _add_dir_to_coord(tail.point_coord, body_dir);
    }
}

Snake::~Snake()
{
    this->pauseExistence();
    LOGGER::log_trace("Snake #{} being removed", this->_id); // be sure the 2nd param is by copy, and not reference, latter case it will be dangling
}

void SnakeBody::grow(Direction move_dir)
{
    snake_ptr->head.graph_box = snake_ptr->head.graph_box->get_adj_box(move_dir);
    snake_ptr->_add_dir_to_coord(snake_ptr->head.point_coord, move_dir);

    body.push_front(
        move_dir    // Edit: No more using util::oppositeDirection
    );

    // tail doesn't change in case of growth
}

void SnakeBody::move(Direction move_dir)
{
    this->grow(move_dir);

    const auto& tail_dir = this->body.back();
    snake_ptr->tail.graph_box = snake_ptr->tail.graph_box->get_adj_box(tail_dir);
    snake_ptr->_add_dir_to_coord(snake_ptr->tail.point_coord, tail_dir);

    this->body.pop_back();
}

    // doesn't remove the head
void SnakeBody::removeAndClearBody() {
    auto tmp_box = this->snake_ptr->head.graph_box;

    for (auto& dir : this->body)
    {
        tmp_box = tmp_box->get_adj_box(dir);
        tmp_box->getDataRef().remEntity(snake_ptr);
    }

    snake_ptr->tail = snake_ptr->head;

    return body.clear();
}

SnakeBody::SnakeBody(Snake* snake_ptr) : snake_ptr(snake_ptr) {}

SnakeState::SnakeState(const Snake* snake):
    EntityState(Entity_Types::SNAKE),
    location(snake->head.point_coord),
    body(snake->body.begin(), snake->body.end())
{}
