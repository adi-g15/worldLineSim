#include "path_finder.hpp"
#include "world.hpp"

#include <vector>

Path PathFinder::getPath( const Snake* snake, _coord food_pos){ // use the snake to get the parent world
    //use some algo for shortest path from snake.head to food_pos

    return {};
}

bool PathFinder::pathIsClear( const World_Ptr game, const Path& p){
    for (auto &&point : p)
    {
        if( ! game->isCellEmpty( point ) )    return false;
    }

    return true;
}
