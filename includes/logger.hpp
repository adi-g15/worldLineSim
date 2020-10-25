#pragma once

#include "curses.h"
#include "declarations.hpp"

#include <mutex>

/*
NOTE - For now, i will be making its methods static,
though later on a logger object can be local member to each object of `Verse` class,
since that would allow something like a `Multiverse`, in each of which only one world will be active,
but each Verse's current World will all keep working
*/
class LOGGER{  //actual logger class that logs the data to a file, or some stream
    static SCREEN* _logger_screen;
    static const SCREEN* _verse_screen;  //will need this to store to use set_term() (in case used later in log_it())

    static std::mutex _screen_mutex;

    public:
        //@log - Or have it take multiple
        static void log_it(uint16_t world_id, Event event, Log& log_obj){
            std::scoped_lock s(_screen_mutex);
            // @todo

        }

        static void startLogger( const SCREEN* verse_screen ){
            // @note - Will likely create a new window and store
            // _logger_screen = newterm();
            _verse_screen = verse_screen;
            // logger_screen_mutex
            // @todo

        }

};
