/*
FOR THE WORLD TO BE DYNAMICALLY GROWING ->
	We need the logica; matrix like structure but list like additions too...  if that's possible

	ACCOMPLISHED -> That is what i wrote the Graph_Mat for !!
*/

#pragma once

#include "log.hpp"
#include "snake.hpp"
#include "world_plot.hpp"

#include <vector>
#include <utility>
#include <unordered_map>
#include <thread>
#include <mutex>

typedef World* World_Ptr;
class World: public _ID{
	typedef int32_t dimen_t;
	typedef std::make_unsigned_t<int32_t> udimen_t;
	typedef _coord<dimen_t> coord_type;
public:
	_timePoint currentTime;

	void ateFood(const Snake*); //which snake ate it, log it, then randomize the food again

	//------constants for this world------//
	int _MAX_NumSnakes = 4;
	int _init_SnakeLength = 2;
	//------                ------//

	const dimen_t& get_curr_bound() const;
	dimen_t getFreeSpace() const; //returns num of boxes empty
	bool isCellEmpty( const coord_type& ) const;

	// --Just abstracted access to private worldPlot member function, no logic in these of their own-- //
	bool isPathClear( const Graph_Box<_box>* origin, const directionalPath& path ) const;
	directionalPath getShortestPathToFood( const Graph_Box<_box>* origin ) const;
	void getShortestPathToFood( const Graph_Box<_box>* origin, directionalPath& ) const;
	// x-Just abstracted access to private worldPlot member function, no logic in these of their own-x //

	struct {
		// @CAUTION - Ensure this access is thread safe
		// this will only be SET IN CONSTRUCTOR AND STOP_SIMULATION, for GET, use the method
			bool _world_runnning{true}; //world will keep moving forward (ie. entities will keep existing and acting)
		public:
			auto is_world_running() const{ return _world_runnning; }
			auto get_world_thread_id() const{ return std::this_thread::get_id(); }
	} _shared_concurrent_data;

	//wil be required to join these threads, in stopSimulation();
	std::vector< std::thread > entity_threads;  // not a concurrently access data, since ONLY to be used by stopSimulation and startSimulation()

	World( const World_Ptr, _timePoint );  //can later be made private

private:
	// std::vector<Log> logs;   // these logs may no longer be required and managed by world_node
	State currentState;
	bool simulationRunning;

	std::mutex __world_mutex;

	// std::pair<uint16_t, uint16_t> _WorldDimens; //current dimensions of this world
	// std::pair<uint16_t, uint16_t> _curr_BOUNDS; //current `reserved` dimensions of this world
	udimen_t _WorldDimen; //current `order` of this world
	udimen_t _curr_BOUND; //current `reserved` `order` of this world

	WorldPlot world_plot;    // @todo - Will be 3D in future

	std::vector< Snake > snakes;

	// bool _CheckBounds();    //for checking `need` to increase size

	bool _RangeCheck(const coord_type&) const;    //for checking if a coordinate is valid

	void runNextUnitTime();   //resumes the world, the nextTime period happens in this time
	World() = default;

	// friend class Verse;  // doesn't need to be a friend, since World_Node is the one that needs that private constructor
	friend class World_Node;

};
