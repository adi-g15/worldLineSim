#include "snake.hpp"
#include "world.hpp"
#include "declarations.hpp"
#include "random.hpp"

#include <thread>
#include <chrono>

void Snake::_Action1(){
    this->eatFood();    // @note - Ignoring returned boolean from eatFood(), if that's required use eatFood() directly, this method is just for the sake for generalisation using Entity
}

void Snake::_Action2(){
    this->moveForward();
}

void Snake::simulateExistence(){
    // @future -> Can add more logic here, when more interaction options between entities are added

    if( std::this_thread::get_id() == this->parent_world.__world_thread_id ){
        #ifdef NO_THREAD_ENTITY
            continue;
        #endif
        throw std::exception( "Entities should be on a different thread, than the world. In case, you don't want this behaviour, then pass -DNO_THREAD_ENTITY (To define NO_THREAD_ENTITY)" );
    }
    while ( this->parent_world->_shared_concurrent_data.is_world_running() ){   //while the parent world continues to exist keep the entity moving
        this->moveForward();

        #ifdef NO_THREAD_ENTITY
            continue;   // don't pause the current thread, if it's the main thread itself
        #endif
        std::this_thread::sleep_for(std::chrono::milliseconds( statics::UNIT_TIME * 1000 ));

    }
}

bool Snake::eatFood(){  //can only eat, if AT the position
    if( this->head->getCoords() == this->parent_world->food ){
        this->parent_world->ateFood(this);
        ++ this->length;
        return true;
    }
    return false;
}

//Returns if `this` snake has ate the food or not
bool Snake::moveForward(){
    if( this->curr_path.empty() ){
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

const coord_type Snake::getHeadCoord() const{
    return this->head->getCoords();
}

const Graph_Box<_box>* Snake::getHead() const{
    return this->head;
}

Graph_Box<_box>* Snake::getHead() {
    return this->head;
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

Snake::Snake(const World_Ptr world, uint16_t init_len) : Entity("Snake"), parent_world(world){
    this->head.mX = Random::random(parent_world->get_curr_bounds().first);
    this->head.mY = Random::random(parent_world->get_curr_bounds().first);

    while( !this->parent_world->isCellEmpty(this->head) ){
        this->head.mX = Random::random(parent_world->get_curr_bounds().first);
        this->head.mY = Random::random(parent_world->get_curr_bounds().first);
    }

    this->body.push_back(this->head);
    for( uint16_t i = 0; i < init_len - 1; i++ ){
        auto tmp = Random::rand_neighbour(this->body.back());
        while( !this->parent_world->isCellEmpty(tmp) ){
            tmp = Random::rand_neighbour(this->body.back());
        }
        this->body.push_back(tmp);
    }


    this->length = init_len;
}

// Snake::Snake(){

// }
