#include "world_plot.hpp"
#include "graphMat/graph_mat.hpp"   // for the definitions
#include "world.hpp"

void WorldPlot::createFood(){
    // Resets the current food; This function should only be called after World::eatFood()

    // @note - I may want to pause the entities to pause for the moment, since further movements may cause the return_box_if_empty function to cause problems, but i will keep moving for now, likely won't change

    std::vector<coord> tmp_list;
    for (const auto& en : this->parent_world->snakes) {
        tmp_list.push_back(en.getPrimaryPos().value()); // directly used optional<>::value since entities depending on this food will have it, as of now
    }

    coord new_coord{ Food::get_new_food_pos(std::move(tmp_list)) };
    auto* box_node = this->return_nearby_empty_box( new_coord );

    bool flag;
    while ( ! box_node )
    {
        flag ? new_coord.mX -= 2: new_coord.mY -= 2;    // @caution - tries out only the bottom left nodes in linear manner
        box_node = this->return_nearby_empty_box(new_coord);
        flag = !flag;
    }

    this->food.reset({ box_node, new_coord });
}

inline const WorldPlot::graph_box_type* WorldPlot::return_nearby_empty_box(const coord& box_coord) const
{
    const graph_box_type* box = this->operator[](box_coord);

    if (!box->getData().hasEntities())   return box;
    else
    {
        // max 9 surrounding blocks will be considered, else return a null pointer
        std::initializer_list<graph_box_type*> surrounding_nodes({
            box->get_adj_box(Direction::NAIRUTYA),
            box->LEFT,
            box->get_adj_box(Direction::VAYAVYA),
            box->UP,
            box->get_adj_box(Direction::ISHANYA),
            box->RIGHT,
            box->get_adj_box(Direction::AGNEYA),
            box->DOWN
        });
        for (graph_box_type* node : surrounding_nodes)
        {
            if (!node && !node->getData().hasEntities()) {
                box = node;
                return box;
            }
        }

        return nullptr;
    }
}

bool WorldPlot::isPathClear( const Graph_Box<_box>* origin, const directionalPath& path ) const{
    const Graph_Box<_box> *temp{ origin };

    for ( uint32_t i = 0; i<path.size(); ++i ){
        temp = temp->get_adj_box(path[i]);
        if( ! temp->getData().entities.empty() )    return false;
    }

    return true;
}

WorldPlot::WorldPlot(const World_Ptr world): Square_Matrix(statics::init_Bound), parent_world(world), food(nullptr){
    this->createFood(); // @caution - food can't be 0,0 handle that

}

void WorldPlot::start_auto_expansion(){
    // @assert - Making sure the world_plot expands on a different thread, so that it doesn't hinder the world thread (it may well be changed later, read the note in world.cpp where this function is called)
    if( std::this_thread::get_id() == this->parent_world->_shared_concurrent_data.get_world_thread_id() ){
        throw std::logic_error("World Plots should be on a different thread than the world thread, so that they don't block the world thread itself");
    }
    this->auto_expand();
}

WorldPlot::dimen_t WorldPlot::getFreeSpace() const{ // returns num of boxes empty
    // @todo - return num of boxes empty

    return dimen_t{};
}


void WorldPlot::auto_expand(){   //expands one unit on each side
    this->__temp.free_space_ratio = this->getFreeSpace()/this->getOrder();
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

void WorldPlot::getShortestPathToFood(const Entity_Point& origin, directionalPath& old_path) const {
    if (path_finder.is_path_clean(origin.graph_box, old_path)) {
        return;
    }

    else old_path = this->getShortestPathToFood(origin);
}

directionalPath&& WorldPlot::getShortestPathToFood(const Entity_Point& origin) const{
    // @important @note - snake class expects the path to be like a stack, ie. the first move, will be the last one, which will be popped, then move_on

    return this->path_finder.getPath(origin);
}

coord&& Food::get_new_food_pos(std::vector<coord>&& entity_positions)
{
    /**
     * @note -> Here the new food position is DEPENDENT on the entity positions
     *          I would better like to have everything as independent as can can be, so that it really simulats a free `world` (free just in sense of independent processes)
     *          This current strategy is only for a performance perspective, so that the new food is created near to all the entities, and just anywhere, so that the path finding algorithms benefits from this 
     *      
     *          @ I may later change it back to independently randomness, even if it costs some performance overhead
     */

    using statics::dimen_t;

    dimen_t min_x, min_y, max_x, max_y;
    min_x = std::numeric_limits<dimen_t>::max();
    min_y = std::numeric_limits<dimen_t>::max();
    max_x = std::numeric_limits<dimen_t>::min();
    max_y = std::numeric_limits<dimen_t>::min();

    for (const auto& pos : entity_positions) {
        min_x = std::min(pos.mX, min_x);
        min_y = std::min(pos.mY, min_y);

        max_x = std::max(pos.mX, max_x);
        max_y = std::max(pos.mY, max_y);
    }

    min_x -= max_x - min_x;
    min_y -= max_y - min_y;

    max_x += max_x - min_x;
    max_y += max_y - min_y;

    return { util::Random::random(min_x, max_x), util::Random::random(min_y, max_y) };
}

void Food::reset(Food&& food)
{
    this->box = food.box;
    this->coordinate = std::move(food.coordinate);
}
