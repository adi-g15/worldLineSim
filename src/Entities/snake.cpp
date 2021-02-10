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

    // instead of using convar, we could have used promise::set_value_at_thread_exit()
    this->isSimulating = true;  // ysa
    while (this->isSimulating && this->parent_world->_shared_concurrent_data.is_world_running() ){   //while the parent world continues to exist keep the entity moving
        LOGGER::log_it(this->parent_world->_id, Event::Entity_Move/*, Moving Snake #", this->_id*/);

        //this->moveForward();

        std::this_thread::sleep_for( std::chrono::milliseconds( (int)statics::UNIT_TIME * 1000 ) );
    }
    this->isSimulating = false;

    std::this_thread::sleep_for( std::chrono::milliseconds(10) );
    this->sim_convar.notify_one();
}

void Snake::pauseExistence() {
    if (!this->isSimulating) return;

    std::mutex m;   // @note @me - Correct it later (I think it shouldn't be how it works ?)
    std::unique_lock<std::mutex> lock(m);

    this->isSimulating = false;
    this->sim_convar.wait_for(lock, std::chrono::milliseconds(80));
        // this is to prevent infinite loop or waiting, since the next line can cause blocking if the notify_one is earlier than wait()
}

bool Snake::eatFood(){  // @note - The body.grow() needs to be called in caller function
    if (!parent_world->lockFood(this))   return false;   // couldn't eat, food is locked

    this->parent_world->ateFood(this);
    return true;
}

bool Snake::hasRoundTrips() const{
        // since array calls the initializer for its object type, so it is `value initialised IN THIS CASE`, ie 0
    auto& temp_bucket = this->__temp.bucket;
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
void Snake::moveForward(){  // this will also be on the snake's thread, and not the world_thread

    // @todo - Change for tail (Can also move head and tail to snake body, but let moveForward like methods outside)
    if (this->parent_world->world_plot.get_food().box == nullptr)    return;

    this->parent_world->getShortestPathToFood(this->head, this->curr_Path);

    if (this->curr_Path.empty()) {   // if path was empty, ie. we are at the food position
        if(this->eatFood()) body.grow(__temp.last_dir);

        return;
    }

    this->__temp.last_dir = curr_Path.next_dir();
    curr_Path.pop();

    this->head.graph_box = this->head.graph_box->get_adj_box(__temp.last_dir);
    this->_add_dir_to_coord( this->head.point_coord, __temp.last_dir);

    body.move(__temp.last_dir);

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

EntityState Snake::_get_current_state() const
{
    return SnakeState(this);
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

Snake::Snake(const World_Ptr world) : Snake(world, world->_init_SnakeLength){}

/*
@bug/enhancement alert - 
    As per the current code, let's consider a situation,
    Let one snake already on board, eg. the line x=4;

    BUT, what if the snake formed is such that, it is y = 3 (or anything);
    THOUGH, isCellEmpty() will return false, but the actual problem is
    "The next move this new snake moves, will cause the new snake to `collide` with the older one, thereby ending its life in starting itself"
*/
// @note - Entities shoud start existence from the start (constructor) itself... though be sure to handle the case when the food is still being created, since the current logic creates the Entities JUST AFTER the snakes, since the createFood logic currently needs the positions of snakes too
Snake::Snake(const World_Ptr world, uint16_t init_len) :
    Entity(Entity_Types::SNAKE),
    parent_world(world),
    body(this),
    head(nullptr, world->world_plot.getRandomCoord()),
    tail(head)
{
    this->head.graph_box = this->parent_world->get_box(head.point_coord);
    assert(head.graph_box != nullptr);

    this->head.graph_box->getDataRef().addEntity(this); // head is a part of the entity

    this->tail = this->head;

    // @note @future - To optimise path finding later, better modify this to only chose dimensions within some distance from 0,0,0

    this->tail.point_coord = this->head.point_coord;
    this->tail.graph_box = this->head.graph_box;
    do {
#ifdef DEBUG
        if (this->body.length() > 0) std::cout << "Clearing Snake: " << this->_id << '\n';
#endif // DEBUG
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
                LOGGER::log_it_verb(5, "Not enough space could be traversed to allocate snake to that area");
            }

            prev_box->getDataRef().addEntity(this);
            this->body.body.push_back(iter._getLastTurnedDirection().value());  // getLastTurnedDirection() MUST have a value

            LOGGER::log_it_verb(6, "Added unit to snake's body: %d", this->_id);

            this->tail.graph_box = prev_box;
            this->tail.point_coord += iter._getIncrementCoords();
        }

    } while ( !isSnakeBodyOK() );
}

Snake::~Snake()
{
    this->pauseExistence();
}

void SnakeBody::grow(Direction move_dir)
{
    // @todo - Change for tail (Can also move head and tail to snake body, but let moveForward like methods outside)
    body.push_front(
        util::getOppositeDirection(move_dir)
    );
}

void SnakeBody::move(Direction move_dir)
{
    // @todo - Change for tail (Can also move head and tail to snake body, but let moveForward like methods outside)
    this->grow(move_dir);
    this->pop_back();
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
    length(snake->getLength()),
    curr_location(snake->head.point_coord)
{}
