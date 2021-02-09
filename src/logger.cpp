#include "logger.hpp"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

#ifdef DEBUG
	#include <iostream>
	using std::clog;
	#undef LOG_S
	#define LOG_S(num) clog
#endif // DEBUG

void LOGGER::log_it(uint16_t world_id, Event event/*, Log& log_obj*/)
{
	switch (event)
	{
	case Event::Entity_Created:
		LOG_S(INFO) << "[#" << world_id << "] Entity {} Created \n";
		break;
	case Event::Entity_Destroyed:
		LOG_S(0) << "[#" << world_id << "] Entity {} Destroyed \n";
		break;
	case Event::Entity_Move:
		LOG_S(2) << "[#" << world_id << "] Entity {} Moved from {} \n";
		break;
	case Event::Entity_Grow:
		LOG_S(1) << "[#" << world_id << "] Entity {} Grown {} units \n";
		break;
	case Event::World_Created:
		LOG_S(WARNING) << "[#" << world_id << "] World Created: \n";
		break;
	case Event::World_Destroyed:
		LOG_S(WARNING) << "[#" << world_id << "] World Destroyed \n";
		break;
	case Event::World_Expanding:
		LOG_S(3) << "[#" << world_id << "] World Expanding \n";
		break;
	case Event::World_Stopped_Expanding:
		LOG_S(WARNING) << "[#" << world_id << "] World Stopped Expanding \n";
		break;
	case Event::World_Paused:
		LOG_S(WARNING) << "[#" << world_id << "] World Paused \n";
		break;
	case Event::World_Resume:
		LOG_S(WARNING) << "[#" << world_id << "] World Resumed \n";
		break;
	case Event::DESTRUCTION_START:
		LOG_S(ERROR) << "[#" << world_id << "] DESTRUCTION STARTED \n";
		break;
	}
}

void LOGGER::start_logger(int argc, char** argv)
{
	loguru::g_preamble_date = false;
	loguru::g_preamble_time = false;
	//loguru::g_preamble_thread = false;
	loguru::g_preamble_verbose = false;

#ifdef DEBUG
	loguru::g_stderr_verbosity = loguru::Verbosity_MAX;
#endif // DEBUG

	loguru::init(argc, argv);
	loguru::add_file("worldline.simulator.log", loguru::FileMode::Truncate, loguru::Verbosity_MAX);
}
