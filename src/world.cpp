/*
    Get w = getWorld( t - t` )

    if( is_descendence(w, curr) ){
        par = w;
    }else {
        par = curr;
    }

    addNewWorld( par );
*/

#include "world.hpp"
#include "random.hpp"
#include "exceptions.hpp"
#include "util/math.hpp"

#include <algorithm>

void World::ateFood(const Snake*){ //which snake ate it, log it, then randomize the food again
    //log the event, with snakeId

    this->food.reset();
    this->createFood();
}

void World::createFood(){
    this->food.mX = Random::random(this->_curr_BOUNDS.first);
    this->food.mY = Random::random(this->_curr_BOUNDS.second);
}

bool World::_CheckBounds(){  //check if there is a need to expand the world
    if( this->_WorldDimens == statics::MAX_BOUNDS ){
        throw WorldCanTExpand();
    }

    if( this->_WorldDimens >= this->_curr_BOUNDS ){
            // expanding the dimensions x2, in case of square, else chose the minimum of the dimensions to expand( just to get away with a little amount of memory )
        try{
            this->_Expand( std::min(this->_WorldDimens.first, this->_WorldDimens.second) );
        }catch(std::bad_alloc &b){
            throw;
        }
        return true;
    }

    return false;
}

bool World::isCellEmpty( const _coord& c ) const{
    if(!this->_RangeCheck(c))   return false;

    return this->world_plot[c.mX][c.mY].isEmpty;
}

bool World::_RangeCheck( const _coord& c ) const{
    return this->world_plot.size() > c.mX && this->world_plot[c.mX].size() > c.mY;
}

void World::_Expand(uint16_t _dimensions_diff){ //allocates  +dimension_diff for both x axis and y axis
    if( this->_WorldDimens == statics::MAX_BOUNDS ){
        throw WorldCanTExpand();
    }

    this->_WorldDimens.first += _dimensions_diff;
    this->_WorldDimens.second += _dimensions_diff;

    if( this->_WorldDimens > this->_curr_BOUNDS ){
        try{
            this->_curr_BOUNDS.first = util::min( statics::MAX_BOUNDS.first, this->_curr_BOUNDS.first * 2 );
            this->_curr_BOUNDS.second = util::min( statics::MAX_BOUNDS.second, this->_curr_BOUNDS.second * 2 );

            this->world_plot.reserve( this->_curr_BOUNDS.first );
            for(auto &row: this->world_plot)
                row.reserve( this->_curr_BOUNDS.second );

        }catch(std::bad_alloc& e){
            this->_curr_BOUNDS = this->_WorldDimens;

            throw WorldCanTExpand();
        }
    }


}

World::World( const World_Ptr world, _timePoint t ): currentTime(t), currentState({}, this->currentTime){
    // @todo
}

World::World(): currentTime(statics::BIG_BANG_TIME), currentState({}, this->currentTime){
    // @todo
    this->_WorldDimens = statics::init_Max_Bound;

    this->snakes.reserve(this->_MAX_NumSnakes);
    this->snakes.push_back(
        Snake(World_Ptr(this), Random::random(2,5))
    );

    this->currentState = State({}, this->currentTime);

}

const std::pair<uint16_t, uint16_t>& World::get_curr_bounds() const{
    return this->_curr_BOUNDS;
}

void World::runNextUnitTime(){
    if(this->food.isNull()) this->createFood(); //though this if block should NEVER be reached, since ateFood() also creates new Food

    auto tmpState = State({}, this->currentTime);
    for(auto& snake: this->snakes){
        snake.moveForward();
        tmpState.curr_pos.push_back(snake.getHead());
    }

    // @todo Log the event
    this->logs.push_back(Log(tmpState));

    ++this->currentTime;
}
