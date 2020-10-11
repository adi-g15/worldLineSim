#include "snake.hpp"
#include "path_finder.hpp"
#include "world.hpp"
#include "declarations.hpp"
#include "random.hpp"

bool Snake::eatFood(){  //can only eat, if AT the position
    if( head == this->parent_world->food ){    //ERROR - Invalid Use of Incomplete type
        this->parent_world->ateFood(this);
        ++ this->length;
    }
}

//Returns if `this` snake has ate the food or not
bool Snake::moveForward(){
    if( this->curr_path.empty() ){
        if( this->eatFood() ){
            return true;
        }
        this->curr_path = PathFinder::getPath(this, this->parent_world->food);
    } else{
        if( !PathFinder::pathIsClear(this->parent_world, this->curr_path) ) //if path is not clear, search for a new path
            this->curr_path = PathFinder::getPath(this, this->parent_world->food);
    }

    // move with path.back()
    //@todo

}

const _coord& Snake::getHead() const{
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

Snake::Snake(){

}
