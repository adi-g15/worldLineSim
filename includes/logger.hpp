#pragma once

/*
NOTE - For now, i will be making its methods static,
though later on a logger object can be local member to each object of `Verse` class,
since that would allow something like a `Multiverse`, in each of which only one world will be active,
but each Verse's current World will all keep working
*/
class _LOGGER{  //actual logger class that logs the data to a file, or some stream
    public:
        //@log - Or have it take multiple
        // static void log_it(World_Ptr, Event, Log);
};
