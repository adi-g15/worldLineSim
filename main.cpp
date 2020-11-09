#include <iostream>
#include <thread>

#include "verse.hpp"
#include "logger.hpp"

static Verse verse;

int main(int argc, char const *argv[])
{
    // LOGGER::startLogger( verse.displayManager.getScreen() );

    verse.big_bang();

    return 0;
}
