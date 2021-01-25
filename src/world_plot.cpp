#include "world_plot.hpp"
#include "graphMat/graph_mat.hpp"   // for the definitions
#include "world.hpp"

void WorldPlot::createFood(){
    // Resets the current food; This function should only be called after World::eatFood()

    // @note - I may want to pause the entities to pause for the moment, since further movements may cause the return_box_if_empty function to cause problems, but i will keep moving for now, likely won't change

    std::vector<coord> tmp_list;
    for (const auto& en : this->parent_world->snakes) {
        tmp_list.push_back(en.getPrimaryPos().value().get().point_coord); // directly used optional<>::value since entities depending on this food will have it, as of now
    }

    coord new_coord{ Food::get_new_food_pos(std::move(tmp_list)) };
    this->_range_check_coord(new_coord);    // check whether the coord in in the world currently
    auto* box_node = this->return_nearby_empty_box( new_coord );

    bool flag{ false }; // doesn't matter much how it's initialised
    while ( ! box_node )
    {
        flag ? new_coord.mX -= 2: new_coord.mY -= 2;    // @caution - tries out only the bottom left nodes in linear manner
        box_node = this->return_nearby_empty_box(new_coord);
        flag = !flag;
    }

    this->food.reset({ box_node, new_coord });
}

void WorldPlot::_rand_once_createFood()
{
    coord new_coord{
        /*the tmp_list being sent is about the half of each dimension, so by curret get_new_food_pos logic, it will use the WHOLE dimensions (initially small, on smaller order of 10 or 100), and randomly select a point within the boundaries*/
        Food::get_new_food_pos({
            {this->getCurrentOrder() / 2, this->getCurrentOrder() / 2, this->getCurrentOrder() / 2},
            {this->getCurrentOrder() / 2, this->getCurrentOrder() / 2, this->getCurrentOrder() / 2},
            {this->getCurrentOrder() / 2, this->getCurrentOrder() / 2, this->getCurrentOrder() / 2},
        })
    };

    this->_range_check_coord(new_coord);    // check whether the coord in the world currently or not
    auto* box_node = this->return_nearby_empty_box(new_coord);

    bool flag{ false }; // doesn't matter much how it's initialised
    while (!box_node)
    {
        flag ? new_coord.mX -= 2 : new_coord.mY -= 2;    // @caution - tries out only the bottom left nodes in linear manner
        box_node = this->return_nearby_empty_box(new_coord);
        flag = !flag;
    }

    this->food.reset({ box_node, new_coord });
}

void WorldPlot::_range_check_coord(coord& coordinate) const
{
    coordinate.mX %= this->getCurrentOrder() / 2;
    coordinate.mY %= this->getCurrentOrder() / 2;
    coordinate.mZ %= this->getCurrentOrder() / 2;
}

bool WorldPlot::_is_in_range_coord(coord& coordinate) const
{
    return (
        std::abs(coordinate.mX) <= this->getCurrentOrder()/2
        && std::abs(coordinate.mY) <= this->getCurrentOrder()/2
        && std::abs(coordinate.mZ) <= this->getCurrentOrder()/2
        );
}

/**
* @note - Returns the exact position itself, instead of neighbours, if already empty
*/
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
            if ( node && !node->getData().hasEntities()) {
                box = node;
                return box;
            }
        }

        return nullptr;
    }
}

bool WorldPlot::isPathClear( const WorldPlot::graph_box_type* origin, const directionalPath& path ) const{
    const graph_box_type *temp{ origin };

    return ! std::any_of(path.begin(), path.end(), [&](const Direction& dir) {
        temp = temp->get_adj_box(dir);
        return temp->getData().hasEntities();
    });
}

WorldPlot::WorldPlot(const World_Ptr world): Cube_Matrix(statics::init_Bound), parent_world(world), path_finder(this) {

    // @bug - DONT call createFood from this constructor, since this is multi-threaded so can't say if entities exist by now, entities are managed by world currently, let it call this too
    // CALLING THE OVERLOADED CREATEFOOD() that doesn't depend on the position of entities in the world, since it's not sure if the entities have got their head coords or not
    this->_rand_once_createFood();

    this->expansion_flag.store(true);
    //std::thread(&WorldPlot::auto_expansion, this).detach();
}

/**
* @note - Call this function on a different thread, this function itself, isn't responsible for creating any new threads
*/
void WorldPlot::auto_expansion(){
    // @assert - Making sure the world_plot expands on a different thread, so that it doesn't hinder the world thread (it may well be changed later, read the note in world.cpp where this function is called)
    if( std::this_thread::get_id() == this->parent_world->_shared_concurrent_data.get_world_thread_id() ){
        throw std::logic_error("World Plots should be on a different thread than the world thread, so that they don't block the world thread itself");
    }

    while (this->__expansion_state.expansion_flag.load())
    {
        this->_expand_once();

        std::this_thread::sleep_for(std::chrono::milliseconds( static_cast<int>( statics::UNIT_TIME * 1000) ));
    }
}

void WorldPlot::resume_auto_expansion()
{
    if (this->__expansion_state.expansion_flag.load()) return;    // if already expanding, then return

    this->__expansion_state.expansion_flag.store(true);

        // we `start` it again on another thread (with previous expansion metadata still in __expansion_state object)
    std::thread(&WorldPlot::auto_expansion, this).detach();
}

    // not actually number of nodes empty, but a utility function for WorldPlot::expand_once()
WorldPlot::dimen_t WorldPlot::getFreeSpace() const{
    dimen_t min_x{ std::numeric_limits<dimen_t>::max() },
        max_x{ std::numeric_limits<dimen_t>::min() },
        min_y{ std::numeric_limits<dimen_t>::max() },
        max_y{ std::numeric_limits<dimen_t>::min() };

    for (auto& snake : this->parent_world->snakes)
    {
        min_x = std::min(snake.getHead().point_coord.mX, min_x);
        max_x = std::max(snake.getHead().point_coord.mX, max_x);
        min_y = std::min(snake.getHead().point_coord.mX, min_y);
        max_y = std::max(snake.getHead().point_coord.mX, max_y);
    }

    return (this->getOrder() * this->getOrder()) - ( (max_x - min_x)*(max_y - min_y) );
}

    // @note - This method may decide NOT to grow this time too
void WorldPlot::_expand_once(){   // `may` expands one unit on each side

    this->__expansion_state.free_space_ratio = static_cast<float>(this->getFreeSpace())/this->getOrder();
    if( __expansion_state.time_since_speed_updated >= 10 ){
        --__expansion_state.expansion_speed;
        __expansion_state.time_since_speed_updated = 0;

            // @future [Oct22] - The below does the speed resetting part, change it to allow negative expansions
        if(__expansion_state.expansion_speed <= 0) __expansion_state.expansion_speed = statics::init_expansion_speed;
    }

    if ( this->__expansion_state.free_space_ratio > statics::max_free_space ){

        // @log - world doesn't need to auto_expand since reached max_free_space

        return;

    }else if ( this->__expansion_state.free_space_ratio < statics::min_free_space )
    {
        ++__expansion_state.time_since_speed_updated;
        ++__expansion_state.expansion_speed;   // @log increasing the expansion speed
    }

    this->__expand_n_units(static_cast<int>(__expansion_state.expansion_speed));
}

void WorldPlot::__expand_n_units(int8_t n){    //to be used when there's rate
    this->resizeOrder(this->getOrder() + n);
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

// @caution the coords returned, may be out of bound, have a check for that in calling function
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

    if (entity_positions.empty())  throw std::logic_error("Food::get_new_food_pos only returns the position of food, if there are snakes there in the world, in current case, the passed vector of entity_positions was empty !");

    dimen_t
        min_x{ std::numeric_limits<dimen_t>::max() },
        max_x{ std::numeric_limits<dimen_t>::min() },
        min_y{ std::numeric_limits<dimen_t>::max() },
        max_y{ std::numeric_limits<dimen_t>::min() },
        min_z{ std::numeric_limits<dimen_t>::max() },
        max_z{ std::numeric_limits<dimen_t>::min() };

    for (const auto& pos : entity_positions) {
        min_x = std::min(pos.mX, min_x);
        min_y = std::min(pos.mY, min_y);
        min_z = std::min(pos.mZ, min_z);

        max_x = std::max(pos.mX, max_x);
        max_y = std::max(pos.mY, max_y);
        max_z = std::max(pos.mZ, max_z);
    }

    // adding padding of difference between mins and maxs
    min_x -= max_x - min_x;
    min_y -= max_y - min_y;
    min_z -= max_z - min_z;

    max_x += max_x - min_x;
    max_y += max_y - min_y;
    max_z += max_z - min_z;

    const int DIFFERENCE_THRESHHOLD = 5;    // minimum difference that MUST be between each coordinate, if not, can just lead to loops of entities just circling a small area where food will ALWAYS be concentrated, even infinite loop when diff = 0
    if (max_x - min_x < DIFFERENCE_THRESHHOLD) {
        min_x -= DIFFERENCE_THRESHHOLD;
        max_x += DIFFERENCE_THRESHHOLD;
    }
    if (max_y - min_y < DIFFERENCE_THRESHHOLD) {
        min_y -= DIFFERENCE_THRESHHOLD;
        max_y += DIFFERENCE_THRESHHOLD;
    }
    if (max_z - min_z < DIFFERENCE_THRESHHOLD) {
        min_z -= DIFFERENCE_THRESHHOLD;
        max_z += DIFFERENCE_THRESHHOLD;
    }
 
    return { 
        static_cast<dimen_t>(util::Random::random(min_x, max_x)),
        static_cast<dimen_t>(util::Random::random(min_y, max_y)),
        static_cast<dimen_t>(util::Random::random(min_z, max_z))
    };
}

void Food::reset(Food&& food)
{
    this->box = std::move(food.box);
    this->coordinate = std::move(food.coordinate);
}
