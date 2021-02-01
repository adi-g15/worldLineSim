#include <iostream>
#include <thread>

#include "verse.hpp"
#include "logger.hpp"

// put it here, since putting it in id_creator.hpp will give multiple definiton link error (there ARE ways like having it in id_creator.cpp, )
//id_type _ID::_curr_ID = util::Random::random(100000);   // @note - Maybe temporarily, so that ids dont' just start with 0,1,2 and so on

static Verse verse;

int main(int argc, char const *argv[])
{
     //LOGGER::startLogger( verse.displayManager.getScreen() );

    verse.big_bang();

    return 0;
}
