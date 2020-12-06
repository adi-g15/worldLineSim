#include "path_finder.hpp"

	// @note - basic_rect doesn't check if the path is empty or not
directionalPath&& Path_Finder::basic_rect( const Entity_Point& start, const coord& end )	// start is a graph box, only to efficiently check if a path is clean or not
{
	directionalPath path;
	path.reserve( std::abs( start.point_coord.mX - end.mX ) + std::abs(start.point_coord.mY - end.mY) );

	if (end.mX < start.point_coord.mX) {
		path.insert(path.end(), std::abs(start.point_coord.mX - end.mX), Direction::PASHCHIM );
	}else{
		path.insert(path.end(), std::abs(start.point_coord.mX - end.mX), Direction::PURVA);
	}

	if ( end.mY < start.point_coord.mY ) {
		path.insert(path.end(), std::abs(start.point_coord.mY - end.mY), Direction::DAKSHIN);
	}
	else {
		path.insert(path.end(), std::abs(start.point_coord.mY - end.mY), Direction::UTTAR);
	}

	return std::move(path);
}

directionalPath&& Path_Finder::rand_basic_rect(const Entity_Point& start, const coord& end)
{
	directionalPath path ( this->basic_rect(start, end) ) ;

	while ( ! this->is_path_clean(start.graph_box, path) ) {
		std::shuffle(path.begin(), path.end(), util::Random::generator);	// logically the start point and end point will stillbe connected
	}

	return std::move(path);
}

bool Path_Finder::is_path_clean(const Graph_Box<_box>* start, const directionalPath& path)
{
	auto* tmp{ start };	// we don't check for the start position, it already has an entity, ie. Us ! :D

	for (auto& dir : path)
	{
		tmp = tmp->get_adj_box(dir);
		if (tmp->getData().hasEntities())	return false;
	}

	return true;
}

directionalPath&& Path_Finder::shortest_path(Entity& start) // end defaults to food position
{
	directionalPath path;
	// @todo @not_priority
	return std::move(path);
}

directionalPath&& Path_Finder::shortest_path(Entity_Point& start, Graph_Box<_box>& end)
{
	directionalPath path;
	// @todo @not_priority
	return std::move(path);
}

inline directionalPath&& Path_Finder::getPath(Entity& entity, bool shortest)
{
	directionalPath path;
	if (shortest) {
		path = this->shortest_path(entity);
	}
	else
	{
		path = this->basic_rect(entity.getPrimaryPos().value(), this->plot->food.coord);
	}
	if (!is_path_clean(path)) {
		if ( ! shortest ) {	// that is the basic rect path was used, and it can cause problems at times, so now dcall shortest paths to find a new path
			return this->getPath(entity, true);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(400));	// check for a new path after 400ms	(This behaviour may change as more path finder methods are added)
		return this->getPath(entity, true);
	}

	return std::move(path);
}

inline Path_Finder::Path_Finder(WorldPlot* fun_space)
{
}

Path_Finder::~Path_Finder()
{
}