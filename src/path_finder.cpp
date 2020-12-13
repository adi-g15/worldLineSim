#include "path_finder.hpp"
#include "world_plot.hpp"

	// @note - basic_rect doesn't check if the path is empty or not
directionalPath&& Path_Finder::basic_rect( const Entity_Point& start, const coord& end ) const	// start is a graph box, only to efficiently check if a path is clean or not
{
	directionalPath path;
	path.reserve( std::abs( start.point_coord.mX - end.mX ) + std::abs(start.point_coord.mY - end.mY) );

	if (end.mX < start.point_coord.mX) {
		path.data.insert(path.data.end(), std::abs(start.point_coord.mX - end.mX), Direction::PASHCHIM );
	}else{
		path.data.insert(path.data.end(), std::abs(start.point_coord.mX - end.mX), Direction::PURVA);
	}

	if ( end.mY < start.point_coord.mY ) {
		path.data.insert(path.data.end(), std::abs(start.point_coord.mY - end.mY), Direction::DAKSHIN);
	}
	else {
		path.data.insert(path.data.end(), std::abs(start.point_coord.mY - end.mY), Direction::UTTAR);
	}

	if (end.mZ < start.point_coord.mZ) {
		path.data.insert(path.data.end(), std::abs(start.point_coord.mZ - end.mZ), Direction::ADHARASTHA);	// downwards
	}
	else {
		path.data.insert(path.data.end(), std::abs(start.point_coord.mZ - end.mZ), Direction::URDHWA);	// upwards
	}

	return std::move(path);
}

directionalPath&& Path_Finder::rand_basic_rect(const Entity_Point& start, const coord& end) const
{
	directionalPath path ( this->basic_rect(start, end) ) ;

	while ( ! this->is_path_clean(start.graph_box, path) ) {
		std::shuffle(path.begin(), path.end(), util::Random::generator);	// logically the start point and end point will stillbe connected
	}

	return std::move(path);
}

bool Path_Finder::is_path_clean(const Graph_Box_3D<_box>* start, const directionalPath& path) const
{
	auto* tmp{ start };	// we don't check for the start position, it already has an entity_point, ie. Hum khud ! :D

	for (auto& dir : path)
	{
		tmp = tmp->get_adj_box(dir);
		if (tmp->getData().hasEntities())	return false;
	}

	return true;
}

directionalPath&& Path_Finder::shortest_path(const Entity_Point& start) const // end defaults to food position
{
	directionalPath path;
	// @todo @not_priority
	return std::move(path);
}

directionalPath&& Path_Finder::shortest_path(const Entity_Point& start, Graph_Box_3D<_box>& end) const
{
	directionalPath path;
	// @todo @not_priority
	return std::move(path);
}

directionalPath&& Path_Finder::getPath(const Entity_Point& entity_point, bool shortest) const
{
	directionalPath path;
	if (shortest) {
		path = this->shortest_path(entity_point);
	}
	else
	{
		path = this->basic_rect(entity_point, this->plot->food.coordinate);
	}
	if (!is_path_clean(entity_point.graph_box, path)) {
		if ( ! shortest ) {	// that is the basic rect path was used, and it can cause problems at times, so now dcall shortest paths to find a new path
			return this->getPath(entity_point, true);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(400));	// check for a new path after 400ms	(This behaviour may change as more path finder methods are added)
		return this->getPath(entity_point, true);
	}

	return std::move(path);
}

Path_Finder::Path_Finder(WorldPlot* fun_space): plot(fun_space)
{
}

Path_Finder::~Path_Finder()
{
}