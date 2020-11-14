#include "world_plot.hpp"
#include "world.hpp"

void WorldPlot::createFood(){
    // Resets the current food; This function should only be called after World::eatFood()

    struct { dimen_t mX, mY; } randomCoords;
    randomCoords.mX = util::Random::random(this->getOrder());
    randomCoords.mY = util::Random::random(this->getOrder());

    // @todo

}

bool WorldPlot::isPathClear( const Graph_Box<_box>* origin, const directionalPath& path ) const{
    const Graph_Box<_box> *temp{ origin };

    for ( uint32_t i = 0; i<path.size(); ++i ){
        temp = temp->get_adj_box(path[i]);
        if( ! temp->getData()->entities.empty() )    return false;
    }

    return true;
}

WorldPlot::WorldPlot(const World_Ptr world): Square_Matrix(statics::init_Bound), parent_world(world), food(nullptr){
    this->createFood(); // @caution - food can't be 0,0 handle that

}

void WorldPlot::start_auto_expansion(){
    // @assert - Making sure the world_plot expands on a different thread, so that it doesn't hinder the world thread (it may well be changed later, read the note in world.cpp where this function is called)
    if( std::this_thread::get_id() == this->parent_world->_shared_concurrent_data.get_thread_id() ){
        throw std::exception("World Plots should be on a different thread than the world thread, so that they don't block the world thread itself");
    }
    this->auto_expand();
}


void WorldPlot::auto_expand(){   //expands one unit on each side
    this->__temp.free_space_ratio = this->parent_world->getFreeSpace()/this->getOrder();
    if( __temp.time_since_speed_updated >= 10 ){
        --__temp.expansion_speed;
        __temp.time_since_speed_updated = 0;

            // @future [Oct22] - The below does the speed resetting part, change it to allow negative expansions
        if(__temp.expansion_speed <= 0) __temp.expansion_speed = statics::init_expansion_speed;
    }

    if ( this->__temp.free_space_ratio > statics::max_free_space ){
        // @log - world doesn't need to auto_expand since reached max_free_space
        return;
    }else if ( this->__temp.free_space_ratio < statics::min_free_space )
    {
        ++__temp.time_since_speed_updated;
        ++__temp.expansion_speed;   // @log increasing the expansion speed
    }

    this->__expand_n_units(static_cast<int>(__temp.expansion_speed));
}

void WorldPlot::__expand_n_units(int8_t n){    //to be used when there's rate
    this->resize(this->getOrder() + n);
}

int32_t WorldPlot::getCurrentOrder() const{  //size
    return this->getOrder();
}

directionalPath WorldPlot::getShortestPathToFood(const Graph_Box<_box>* origin) const{
    directionalPath shortest_path;
    // this->getShortestPathToFood(std::forward(origin), shortest_path);
    this->getShortestPathToFood(origin, shortest_path);

    return shortest_path;
}

void WorldPlot::getShortestPathToFood(const Graph_Box<_box>* origin, directionalPath& old_path) const{
    old_path.clear();   //the old path will be cleared !! If you wish to be more optimised, call WorldPlot::isPathClear() before calling this function

    // @todo - Apply the BFS here
    // @note - snake class expects the path to be like a stack, ie. the first move, will be the last one, which will be popped, then move_on

}
