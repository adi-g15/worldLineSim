#pragma once

#include <spdlog/async.h>
#include <spdlog/fmt/ostr.h> // for printing custom types
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "db/database.hpp"
#include "declarations.hpp"

namespace LOGGER {
inline void init() {
    db::init();

    spdlog::info("Starting spdlog");
    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>(
        "async_file_logger", "logs/async_log.txt");
    const char *msg = "Hi there";
    spdlog::info(msg);
}

template <typename FirstType, typename... Args>
inline void log_msg(const char *msg_format, FirstType arg, Args... args) {
    spdlog::info(msg_format, std::forward<FirstType>(arg),
                 std::forward<Args>(args)...);
}

inline void log_msg(const char *msg_format) { spdlog::info(msg_format); }

template <typename... Args>
inline void log_trace(const char *msg_format,
                      Args... args) // low importance logs
{
    spdlog::trace(msg_format, std::forward<Args>(args)...);
}

inline void log_imp(uint16_t world_id, Event event) {
    switch (event) {
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

}; // namespace LOGGER
