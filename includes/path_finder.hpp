#pragma once

#include "graphMat/direction.hpp"
#include "world_plot.hpp"
#include "entity.hpp"

class Path_Finder
{
	WorldPlot* plot;

public:
	directionalPath&& getPath(Entity& entity, bool shortest = false);	// if shortest is false, the algorithm is free to chose whichever algo givces the path

	directionalPath&& getPath(Entity& entity, Graph_Box<_box>& final_dest, bool shortest = false) = delete;	/** @not_implemented */
	directionalPath&& getPath(Entity& entity, util::_coord<WorldPlot::dimen_t>& final_dest, bool shortest = false) = delete;	/** @not_implemented */
	bool is_path_clean(const Graph_Box<_box>*, const directionalPath&);

	Path_Finder(WorldPlot* fun_space);
	~Path_Finder();

private:
	/*PATHFINDERS START*/
	// @note - Some private path finders won't check if the path is clear or not, do that yourself, one such is basic_path()
	directionalPath&& basic_rect(const Entity_Point&, const coord&);	// just gives a basic rectangular path with only a single turn
	directionalPath&& rand_basic_rect(const Entity_Point&, const coord&);	// just gives a basic rectangular path with only a single turn

	directionalPath&& shortest_path(Entity&);	/** @not_implemented */	// uses the food position as the ending point
	directionalPath&& shortest_path(Entity_Point&, Graph_Box<_box>&);	/** @not_implemented */
	/*PATHFINDERS END*/

};
