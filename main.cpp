#include <nanogui/nanogui.h>

#include "logger.hpp"
#include "verse.hpp"

int main(int argc, char *argv[]) {
    try {
        LOGGER::init();  // for logging to console
        nanogui::init(); // for display

        Verse verse;
        verse.big_bang();
    } catch (...) { // catch all handler
        return EXIT_FAILURE;
    }

    return 0;
}
