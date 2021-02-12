#pragma once

#include <quill/Quill.h>
#include <quill/Utility.h>
#include "declarations.hpp"

namespace LOGGER{
    inline void init() {
		quill::start();
		quill::init_signal_handler();

		quill::get_logger()->init_backtrace(64, quill::LogLevel::Warning);
	}

	inline void log_imp(uint16_t world_id, Event event) {
		switch (event)
		{
		case Event::Entity_Created:
		case Event::Entity_Destroyed:
		case Event::Entity_Move:
		case Event::Entity_Grow:
		case Event::World_Expanding:
			break;
		case Event::World_Created:
			LOG_DEBUG(quill::get_logger(), "[#{}] World Created", world_id);
			break;
		case Event::World_Destroyed:
			LOG_WARNING(quill::get_logger(), "[#{}] World Destroyed", world_id);
			break;
		case Event::World_Paused:
			LOG_WARNING(quill::get_logger(), "[#{}] World Paused", world_id);
			break;
		case Event::World_Resumed:
			LOG_DEBUG(quill::get_logger(), "[#{}] World Resumed", world_id);
			break;
		case Event::DESTRUCTION_START:
			LOG_ERROR(quill::get_logger(), "[#{}] DESTRUCTION STARTED", world_id);
			break;
		}
	}

    template<typename ...Args>
	inline void log_msg(const char* msg_format, Args ...args) {
		std::clog << msg_format << std::endl;
		LOG_BACKTRACE(quill::get_logger(), "{} arguments: Checking", sizeof...(args), std::forward<Args>( args )... );
		//LOG_BACKTRACE(quill::get_logger(), std::forward < const char* >( msg_format ), std::forward<Args>( args )... );
	}

	template<typename ...Args>
	inline void log_trace(const char* msg_format, Args ...args)	// low importance logs
	{
		//std::clog << msg_format << std::endl;
		//LOG_BACKTRACE(quill::get_logger(), msg_format, std::forward<Args>(args)... );
	}
};
