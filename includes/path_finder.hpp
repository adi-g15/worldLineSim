#pragma once

#include "graphMat/direction.hpp"
#include "entity.hpp"

class WorldPlot;	// forward-decl

class Path_Finder
{
	WorldPlot* plot;

public:
	directionalPath getPath(const Entity_Point& entity, bool shortest = false) const;	// if shortest is false, the algorithm is free to chose whichever algo givces the path

	directionalPath getPath(const Entity_Point& entity, Graph_Box_3D<Box>& final_dest, bool shortest = false) = delete;	/** @not_implemented */
	directionalPath getPath(const Entity_Point& entity, util::_coord3D<statics::dimen_t>& final_dest, bool shortest = false) = delete;	/** @not_implemented */
	bool is_path_clean(const Graph_Box_3D<Box>*, const directionalPath&) const;

	Path_Finder(WorldPlot* fun_space);
	~Path_Finder();

private:
	/*PATHFINDERS START*/
	// @note - Some private path finders won't check if the path is clear or not, do that yourself, one such is basic_path()
	directionalPath basic_rect(const Entity_Point&, const coord&) const;	// just gives a basic rectangular path with only a single turn
	directionalPath rand_basic_rect(const Entity_Point&, const coord&) const;	// just gives a basic rectangular path with only a single turn

	directionalPath shortest_path(const Entity_Point&) const;	/** @not_implemented */	// uses the food position as the ending point
	directionalPath shortest_path(const Entity_Point&, Graph_Box_3D<Box>&) const;	/** @not_implemented */
	/*PATHFINDERS END*/

};
