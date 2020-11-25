#include "snake.hpp"
#include "world.hpp"
#include "declarations.hpp"
#include "util/random.hpp"

#include <thread>
#include <chrono>

void Snake::_Action1(){
    this->eatFood();    // @note - Ignoring returned boolean from eatFood(), if that's required use eatFood() directly, this method is just for the sake for generalisation using Entity
}

void Snake::_Action2(){
    this->moveForward();
}

const coord& Snake::getPos() const{
    return this->getHeadCoord();
}


void Snake::simulateExistence(){
    // @future -> Can add more logic here, when more interaction options between entities are added

    // a one time check, before an entity starts existing
    if( std::this_thread::get_id() == this->parent_world->_shared_concurrent_data.get_world_thread_id() ){
        throw std::logic_error( "Entities should be on a different thread, than the world. In case, you don't want this behaviour, then pass -DNO_THREAD_ENTITY (To define NO_THREAD_ENTITY)" );
    }

    while ( this->parent_world->_shared_concurrent_data.is_world_running() ){   //while the parent world continues to exist keep the entity moving
        this->moveForward();

        std::this_thread::sleep_for( std::chrono::milliseconds( (int)statics::UNIT_TIME * 1000 ) );
    }
}

bool Snake::eatFood(){  //can only eat, if AT the position
    if( this->head->getCoords() == this->parent_world->get_food_coords()){
        this->parent_world->ateFood(this);
        ++ this->length;
        return true;
    }
    return false;
}

//Returns if `this` snake has ate the food or not
bool Snake::moveForward(){  // this will also be on the snake's thread, and not the world_thread

    if( this->curr_Path.empty() ){
        if( this->eatFood() ){
            return true;
        }
        this->parent_world->getShortestPathToFood( this->head, curr_Path );
    } else{
        if( !this->parent_world->isPathClear( head, curr_Path ) ) //if path is not clear, search for a new path
            this->parent_world->getShortestPathToFood( head, curr_Path );
    }

    // move with path.back()
    // @todo

}

const coord& Snake::getHeadCoord() const{
    return this->head->getCoords();
}

const Graph_Box<_box>* Snake::getHead() const{
    return this->head;
}

int Snake::getUniqProp() const{
    return this->length;
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

Snake::Snake(const World_Ptr world, uint16_t init_len) : Entity(entity_Types::SNAKE), parent_world(world){
    coord head_pos = {
        util::Random::random<dimen_t>(parent_world->get_curr_bound()),
        util::Random::random<dimen_t>(parent_world->get_curr_bound()),
    };

    this->head = parent_world->get_box(head_pos);

    while( !this->parent_world->isCellEmpty(this->head) ){  // loop until you chose an empty box as the head
        this->head = this->head->get_adj_box( Direction(util::Random::random<dimen_t>(4)) );    // @warning- randomly assigning any number from 0 to 3
    }

    // this->body.push_back(this->head);    // since it's a list of directions, we don't require the head, its not direction, and also it is the starting point
    for( uint16_t i = 0; i < init_len - 1; i++ ){
        // @todo - Randomly chose a direction out of 4 directions currently
        auto* prev_box = this->head;

        // util::Random::rand_neighbour(this->body.back())
        auto rand_direction = statics::directions[ util::Random::random(4) ];

        while( ! prev_box->getData().hasEntities() ){
            prev_box = prev_box->get_adj_box(Direction::UP);
            rand_direction = statics::directions[ util::Random::random(4) ];
        }
        this->body.push_back(rand_direction);
    }


    this->length = init_len;
}

// Snake::Snake(){

// }
