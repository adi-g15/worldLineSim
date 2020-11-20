#pragma once

#include "includes/snake.hpp"
#include "includes/declarations.hpp"
#include "includes/forward_decl.hpp"

#include <vector>

typedef std::vector<coord> Path;

class PathFinder{
    /*
        The PathFinder functions will need 3 things ->
        1. Start Pos (will be head of snake)
        2. End Pos (will be parent_world->get_food_coords())
        3, Obstacles to consider  (which is major reason for needing world)
    
        Though we don't need to take in the world reference here, since it's already binded with the snake
    */

   typedef World* World_Ptr;
   public:
    static Path getPath( const Snake*, coord&);

    static bool pathIsClear( const World_Ptr, const Path&);

};
