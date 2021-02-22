#pragma once

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h> // for printing custom types

#include "declarations.hpp"
#include "db/database.hpp"

namespace LOGGER{
	inline std::atomic_flag lock = ATOMIC_FLAG_INIT;
	inline std::mutex m = std::mutex();

    inline void init() {
		db::init();

		spdlog::info("Starting spdlog");
		auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.txt");
		const char* msg = "Hi there";
		spdlog::info(msg);

		lock.clear();
	}

	inline void _log_msg(const char* msg_format) {
		std::clog << msg_format << std::endl;

		//m.unlock();
		lock.clear();
		//lock.clear(std::memory_order_release);
	}

	template<typename FirstType, typename ...Args>
	inline void _log_msg(const char* msg_format, FirstType arg, Args ...args) {
		//while (lock.test_and_set())  // acquire lock
		//	; // spin

		for (; *msg_format != '\0'; msg_format++) {
			if (*msg_format == '{' && *(msg_format + 1) == '}') {
				std::clog << arg;
				_log_msg(msg_format + 2, std::forward<Args>(args)...); // recursive call
				return;
			}
			std::clog << *msg_format;
		}

		lock.clear();
	}

	template<typename FirstType, typename ...Args>
	inline void log_msg(const char* msg_format, FirstType arg, Args ...args) {
		spdlog::info(msg_format, std::forward <FirstType>(arg), std::forward<Args>(args)...);
	}

	inline void log_msg(const char* msg_format) {
		return _log_msg(msg_format);
	}

	template<typename ...Args>
	inline void log_trace(const char* msg_format, Args ...args)	// low importance logs
	{
		spdlog::trace(msg_format, std::forward<Args>(args)...);
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
			spdlog::warn("[#{}] World Created", world_id);
			break;
		case Event::World_Destroyed:
			spdlog::warn("[#{}] World Destroyed", world_id);
			break;
		case Event::World_Paused:
			spdlog::warn("[#{}] World Paused", world_id);
			break;
		case Event::World_Resumed:
			spdlog::warn("[#{}] World Resumed", world_id);
			break;
		case Event::DESTRUCTION_START:
			spdlog::critical("[#{}] DESTRUCTION STARTED", world_id);
			break;
		}
	}

};
