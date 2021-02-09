#include <iostream>
#include <thread>

#include <nanogui/nanogui.h>

#include "verse.hpp"
#include "logger.hpp"

using namespace nanogui;

int main(int argc, char *argv[])
{
    LOGGER::start_logger(argc, argv);

    nanogui::init();

    Verse verse;
    verse.big_bang();

    return 0;
}
