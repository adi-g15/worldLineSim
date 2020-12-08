#pragma once

#include "graphMat/graph__square_mat_decl.hpp"
#include "declarations.hpp"
#include "forward_decl.hpp"
#include "path_finder.hpp"
#include <atomic>	// For std::atomic_bool (actually not mandatory, some extra loops are okay in auto_expand)

/*MUST ALWAYS BE ON A SINGLE THREAD AT MAX, SINCE TEMPORARY VARIABLES ARE USED AS MEMBER OF THIS CLASS*/

struct Food {
	const Graph_Box<_box>* box;   // @future - food may have different points/nutitional values too
	coord coordinate;	// food and entities will be having coordinates

private:
	static coord&& get_new_food_pos(std::vector<coord>&& entity_pos);
	void reset(Food&& food);

	Food() = default;
	Food(Food&) = default;
	Food(Food&&) = default;
	friend class WorldPlot;
	friend class World;
};

// @future - If it lives on its own thread, put a auto expansion logic, that sleeps for 1 time unit, and then calls auto_expand()
class WorldPlot: public Square_Matrix<_box>{
	typedef int32_t dimen_t;
	typedef World* World_Ptr;
	typedef Graph_Box<_box> graph_box_type;

	const World_Ptr parent_world;
	Path_Finder path_finder;
	Food food;   // @future - food may have different points/nutitional values too

	std::atomic_bool expansion_flag;
	void auto_expansion();	 //keeps expanding TILL expansion_flag is TRUE
	void pause_auto_expansion();
	void resume_auto_expansion();
	dimen_t getFreeSpace() const;
	void _expand_once();
	void __expand_n_units(int8_t n);    //to be used when there's rate
	const Graph_Box<_box>* get_box(const coord& position) const{
		// @todo - Return the graph_box with that coordinate

		return &(this->origin);    // @debug - just for now
	}
	directionalPath&& getShortestPathToFood(const Entity_Point& origin) const;

	public:
		const auto& get_food() const{
			return this->food;
		}
		void createFood();
		dimen_t getCurrentOrder() const;

		const graph_box_type* return_nearby_empty_box(const coord& box_coord) const;

		// @future - For optimising purpose, use the food coords in parent_world (eg. to go search the direction which has the food, for eg, if it is in a coord on right, only iterate through those)
		void getShortestPathToFood(const Entity_Point& origin, directionalPath&) const;

		struct {
			float expansion_speed{ statics::init_expansion_speed }; //initially it will auto_expand statics::init_expansion_speed unit at time, each side
			float free_space_ratio{100.0f};

			_timePoint time_since_speed_updated{0}; //after 10 time units, the __temp.expansion_speed will be decremented/reset, so as to not unecessary keep increasing storage
		} __temp;   //temporary vars


	// @note - The world_plot starts AUTO EXPANSION, from constructor itself
	WorldPlot(const World_Ptr);

	friend class World;
	friend class Path_Finder;	// it will be a friend of world_plot too, so as to control its ability to auto expand

};
