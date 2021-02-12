#include <nanogui/nanogui.h>

#include "verse.hpp"
#include "logger.hpp"

int main(int argc, char *argv[])
{
    LOGGER::init(); // for logging to console
    nanogui::init();    // for display

    Verse verse;
    verse.big_bang();

    return 0;
}
