#pragma once

#include "log.hpp"
#include "declarations.hpp"

// UPDATE: 3rd Feb - Will be using the console for logging

/*
NOTE - For now, i will be making its methods static,
though later on a logger object can be local member to each object of `Verse` class,
since that would allow something like a `Multiverse`, in each of which only one world will be active,
but each Verse's current World will all keep working
*/
class LOGGER{
    //static inline std::ostream& log_stream = std::clog;

    public:
        template<typename... Args>
        static void log_it_verb(int8_t verbosity_level, const char* msg, Args... args);  // ONLY call this for low verbosity
        static void log_it(uint16_t world_id, Event event/*, Log& log_obj*/);
        static void start_logger(int, char**);
};

template<typename ...Args>
inline void LOGGER::log_it_verb(int8_t verbosity_level, const char* msg, Args ...args)
{
    //loguru::log(verbosity_level, stderr, 10, msg);

    return;
}
