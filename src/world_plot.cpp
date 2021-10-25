#include "world_plot.hpp"
#include "graphMat/iterators.hpp"
#include "logger.hpp"
#include "world.hpp"

std::mt19937 util::Random::generator;

void WorldPlot::createFood() {
    // Resets the current food; This function should only be called after
    // World::eatFood()
    this->food.reset({nullptr, {0, 0, 0}});

    std::vector<coord> tmp_list;
    for (const auto &en : this->parent_world->entities) {
        if (en->type == Entity_Types::SNAKE)
            tmp_list.push_back(
                en->getPrimaryPos()
                    .value()
                    .point_coord); // directly used optional<>::value since
                                   // entities depending on this food will have
                                   // it, as of now
    }

    coord new_coord{Food::get_new_food_pos(std::move(tmp_list))};
    this->_fit_coord_in_range(
        new_coord); // check whether the coord in in the world currently
    auto *box_node = this->return_nearby_empty_box(new_coord);

    bool flag{false}; // doesn't matter much how it's initialised
    while (!box_node) {
        flag ? new_coord.mX -= 2
             : new_coord.mY -= 2; // @caution - tries out only the bottom left
                                  // nodes in linear manner
        box_node = this->return_nearby_empty_box(new_coord);
        flag = !flag;
    }

    this->food.reset({box_node, new_coord});
}

void WorldPlot::createFood(const coord &food_pos) {
    this->food.reset({this->operator[](food_pos), food_pos});
}

// COME HERE
void WorldPlot::_rand_once_createFood() {
    coord new_coord{
        /*the tmp_list being sent is about the half of each dimension, so by
           curret get_new_food_pos logic, it will use the WHOLE dimensions
           (initially small, on smaller order of 10 or 100), and randomly select
           a point within the boundaries*/
        Food::get_new_food_pos({
            {this->getOrder() / 2, this->getOrder() / 2, this->getOrder() / 2},
            {this->getOrder() / 2, this->getOrder() / 2, this->getOrder() / 2},
            {this->getOrder() / 2, this->getOrder() / 2, this->getOrder() / 2},
        })};

    this->_fit_coord_in_range(
        new_coord); // being sure that coordinate is in range
    auto *box_node = this->return_nearby_empty_box(new_coord);

    bool flag{false}; // doesn't matter much how it's initialised
    while (!box_node) {
        flag ? new_coord.mX -= 2
             : new_coord.mY -= 2; // @caution - tries out only the bottom left
                                  // nodes in linear manner
        box_node = this->return_nearby_empty_box(new_coord);
        flag = !flag;
    }

    this->food.reset({box_node, new_coord});
}

const util::_coord3D<int> &WorldPlot::_get_internal_dimensions() const {
    return this->total_abs;
}
void WorldPlot::_fit_coord_in_range(coord &coordinate) const noexcept {
    coordinate.mX %= this->getOrder() / 2;
    coordinate.mY %= this->getOrder() / 2;
    coordinate.mZ %= this->getOrder() / 2;
}

bool WorldPlot::_is_in_range_coord(const coord &coordinate) const noexcept {
    return (coordinate.mX >= this->min_x && coordinate.mX <= this->max_x) &&
           (coordinate.mY >= this->min_y && coordinate.mY <= this->max_y) &&
           (coordinate.mZ >= this->min_z && coordinate.mZ <= this->max_z);
}

/**
 * @note - Returns the exact position itself, instead of neighbours, if already
 * empty
 */
inline const WorldPlot::graph_box_type *
WorldPlot::return_nearby_empty_box(const coord &box_coord) const {
    graphMat::NeighbourIterator<Box, true> iter(this->operator[](box_coord));

    for (; iter.center_box != nullptr; ++iter) {
        if (!iter->getData().hasEntities())
            return iter.curr_box;
    }

    // this will be much more efficient if the matrix used uses `caching` of
    // last accessed nodes
    constexpr std::array<std::array<coord::type, 3>, 10>
        increments_to_neighbour_boxes = {
            std::array{-2, 0, 0}, {2, 0, 0}, {0, -2, 0}, {0, 2, 0}, {-2, -2, 0},
            {-2, 2, 0},           {2, 2, 0}, {2, -2, 0}, {0, 0, 2}, {0, 0, -2}};
    for (auto &increment_coord :
         increments_to_neighbour_boxes) // we will try looking at neighbours of
                                        // neighbouring boxes (not direct
                                        // neighbours, since that would repeat
                                        // for many already visited nodes)
    {
        const auto retVal =
            this->return_nearby_empty_box(box_coord + increment_coord);

        if (retVal != nullptr)
            return retVal;
    }

    return nullptr;
}

bool WorldPlot::isPathClear(const WorldPlot::graph_box_type *origin,
                            const directionalPath &path) const {
    const graph_box_type *temp{origin};

    return std::none_of(path.begin(), path.end(), [&](const Direction &dir) {
        temp = temp->get_adj_box(dir);
        return temp->getData().hasEntities();
    });
}

WorldPlot::WorldPlot(const World_Ptr world)
    : Cube_Matrix(statics::init_Bound), parent_world(world), path_finder(this) {
    this->_rand_once_createFood();
    this->resume_auto_expansion();
}

WorldPlot::WorldPlot(const World_Ptr world, const State &start_state)
    : Cube_Matrix(start_state.universe_order), parent_world(world),
      path_finder(this) {
    this->createFood(start_state.food_pos);
    // this->_rand_once_createFood();
    this->resume_auto_expansion();
}

void WorldPlot::resume_auto_expansion() {
    if (this->__expansion_state.expansion_flag.load())
        return; // if already expanding, then return

    this->__expansion_state.reset_initializer();
    this->__expansion_state.expansion_flag.store(true);

    // we `start` it again on another thread (with previous expansion metadata
    // still in __expansion_state object)
    std::thread(&WorldPlot::auto_expansion, this).detach();
}

/**
 * @note - Call this function on a different thread, this function itself, isn't
 * responsible for creating any new threads
 */
void WorldPlot::auto_expansion() {
    this->__expansion_state.expansion_flag = true;
    this->__expansion_state.is_actively_expanding = true;

    while (this->__expansion_state.expansion_flag) {
        LOGGER::log_msg("World #{} expanding from order {}",
                        this->parent_world->_id, this->getOrder());
        this->expand_once();

        // sleep for 1 unit time
        std::this_thread::sleep_for(statics::UNIT_TIME);
    }

    this->__expansion_state.is_actively_expanding = false;
    LOGGER::log_imp(this->parent_world->_id, Event::World_Stopped_Expanding);
}

void WorldPlot::pause_auto_expansion() {
    // return if not expanding, or "shouldn't" be auto expanding now
    if (!this->__expansion_state.expansion_flag)
        return;

    this->__expansion_state.expansion_flag.store(false);

    while (this->__expansion_state.is_actively_expanding) {
    }
}

// not actually number of nodes empty, but a utility function for
// WorldPlot::expand_once()
float WorldPlot::getFreeSpaceRatio() const {
    dimen_t min_x{std::numeric_limits<dimen_t>::max()},
        max_x{std::numeric_limits<dimen_t>::min()},
        min_y{std::numeric_limits<dimen_t>::max()},
        max_y{std::numeric_limits<dimen_t>::min()},
        min_z{std::numeric_limits<dimen_t>::max()},
        max_z{std::numeric_limits<dimen_t>::min()};

    for (auto &entity : this->parent_world->entities) {
        if (entity->type != Entity_Types::ROCK &&
            entity->getPrimaryPos().has_value()) {
            min_x =
                std::min(entity->getPrimaryPos().value().point_coord.mX, min_x);
            max_x =
                std::max(entity->getPrimaryPos().value().point_coord.mX, max_x);
            min_y =
                std::min(entity->getPrimaryPos().value().point_coord.mY, min_y);
            max_y =
                std::max(entity->getPrimaryPos().value().point_coord.mY, max_y);
            min_z =
                std::min(entity->getPrimaryPos().value().point_coord.mZ, min_z);
            max_z =
                std::max(entity->getPrimaryPos().value().point_coord.mZ, max_z);
        }
    }

    if (((min_x - this->min_x) > 16) && ((min_y - this->min_y) > 16) &&
        ((min_z - this->min_z) > 16) && ((this->max_x - max_x) > 16) &&
        ((this->max_y - max_y) > 16) && ((this->max_z - max_z) > 16)) {
        return statics::min_free_space - 1;
    } else
        return 100.0f - static_cast<float>((max_x - min_x) * (max_y - min_y) *
                                           (max_z - min_z)) /
                            (this->total_abs.mX * total_abs.mY * total_abs.mZ);
}

// @note - The function decides how much to grow, and may decide to not grow at
// all "for this call"
void WorldPlot::expand_once() { // `may` expands one unit on each side

    constexpr float decrease_rate = 0.90f; // 90% of previous expansion speed

    const auto free_space_ratio = this->getFreeSpaceRatio();

    // @future [Oct22] - The below does the speed resetting part, change it to
    // allow negative expansions
    if (this->__expansion_state.time_since_speed_updated % 10 == 0) {
        this->__expansion_state.speed_doubled_recently = false;
        this->__expansion_state.curr_expansion_speed =
            this->__expansion_state.expansion_speed;
        this->__expansion_state.time_since_speed_updated = 0;
    }

    if (free_space_ratio > statics::max_free_space) {

        // @log - world doesn't need to auto_expand since reached max_free_space

        return;
    } else if (free_space_ratio < statics::min_free_space &&
               !this->__expansion_state.speed_doubled_recently) {
        this->__expansion_state.curr_expansion_speed *=
            2; // @log temporarily doubling the expansion speed
        this->__expansion_state.speed_doubled_recently = true;
    } else {
        this->__expansion_state.curr_expansion_speed *= decrease_rate;
    }

    this->__expansion_state.increase_units +=
        this->__expansion_state.curr_expansion_speed;

    const int int_part{static_cast<int>(__expansion_state.increase_units)};
    this->expand_n_unit(int_part);

    this->__expansion_state.increase_units -= int_part;

    ++__expansion_state.time_since_speed_updated;
}

coord WorldPlot::getRandomCoord() const noexcept {
    return {std::clamp(rand() % this->total_abs.mX, this->min_x, this->max_x),
            std::clamp(rand() % this->total_abs.mY, this->min_y, this->max_y),
            std::clamp(rand() % this->total_abs.mZ, this->min_z, this->max_z)};
}

void WorldPlot::getShortestPathToFood(const Entity_Point &origin,
                                      directionalPath &old_path) const {
    if (!old_path.empty() &&
        path_finder.is_path_clean(origin.graph_box, old_path)) {
        return;
    }

    else
        old_path = this->getShortestPathToFood(origin);
}

inline directionalPath
WorldPlot::getShortestPathToFood(const Entity_Point &origin) const {
    // @important @note - snake class expects the path to be like a stack, ie.
    // the first move, will be the last one, which will be popped, then move_on

    return this->path_finder.getPath(
        origin); // by default, doesn't return the `shortest path`
}

// @caution the coords returned, may be out of bound, have a check for that in
// calling function
coord Food::get_new_food_pos(const std::vector<coord> &entity_positions) {
    /**
     * @note -> Here the new food position is DEPENDENT on the entity positions
     *          I would better like to have everything as independent as can can
     * be, so that it really simulats a free `world` (free just in sense of
     * independent processes) This current strategy is only for a performance
     * perspective, so that the new food is created near to all the entities,
     * and just anywhere, so that the path finding algorithms benefits from this
     *
     *          @ I may later change it back to independently randomness, even
     * if it costs some performance overhead
     */

    using statics::dimen_t;

    if (entity_positions.empty())
        throw std::logic_error(
            "Food::get_new_food_pos only returns the position of food, if "
            "there are snakes there in the world, in current case, the passed "
            "vector of entity_positions was empty !");

    dimen_t min_x{std::numeric_limits<dimen_t>::max()},
        max_x{std::numeric_limits<dimen_t>::min()},
        min_y{std::numeric_limits<dimen_t>::max()},
        max_y{std::numeric_limits<dimen_t>::min()},
        min_z{std::numeric_limits<dimen_t>::max()},
        max_z{std::numeric_limits<dimen_t>::min()};

    for (const auto &pos : entity_positions) {
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

    const int DIFFERENCE_THRESHHOLD =
        5; // minimum difference that MUST be between each coordinate, if not,
           // can just lead to loops of entities just circling a small area
           // where food will ALWAYS be concentrated, even infinite loop when
           // diff = 0
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

    return {static_cast<dimen_t>(util::Random::random(min_x, max_x)),
            static_cast<dimen_t>(util::Random::random(min_y, max_y)),
            static_cast<dimen_t>(util::Random::random(min_z, max_z))};
}

void Food::reset(Food &&food) {
    this->box = std::move(food.box);
    this->coordinate = std::move(food.coordinate);
}
