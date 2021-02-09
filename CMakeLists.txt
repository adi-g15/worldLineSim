cmake_minimum_required(VERSION 3.10.0)
project(WorldLineSim
    DESCRIPTION "WorldLine Simulator"
    LANGUAGES CXX
    VERSION 0.271
    )
include(CTest)

# Later utilise features in C++20
set(CMAKE_CXX_STANDARD 17)

if(NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/include")
    message(FATAL_ERROR "The nanogui dependency is missing!! "
            "You might not have cloned the repo with --recursive "
            "You will need to clone the dependencies too, use \`git submodule --init --recursive\`"
        )
endif()

add_subdirectory(ext/loguru)
add_subdirectory(ext/nanogui)

include_directories(ext/graphMat/includes/)
include_directories(ext/utilpp/includes/)
include_directories(ext/loguru/)

file(GLOB SOURCES "src/Entities/*.cpp" "src/*.cpp")

#add_compile_definitions(GRAPH_MAT_NO_COORD)
add_compile_definitions(DEBUG)
add_executable(simulator main.cpp ${SOURCES})
target_link_libraries(simulator PRIVATE loguru)
target_link_libraries(simulator PRIVATE display)

include_directories(includes)
include_directories(.)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB DISP_SOURCES "display/src/*.cpp")
add_library(display STATIC ${DISP_SOURCES})
include_directories(ext/nanogui/include)
target_link_libraries(display PRIVATE nanogui)

set(NANOGUI_BUILD_EXAMPLES  OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(NANOGUI_BUILD_PYTHON OFF CACHE BOOL " " FORCE)

target_link_libraries(simulator PRIVATE Threads::Threads)
target_link_libraries(simulator PRIVATE display)

# set(CMAKE_TOOLCHAIN_FILE "C:/Users/adity/libs/vcpkg/scripts/buildsystems/vcpkg.cmake")
# later distribute display into a library, and link to it
# file(GLOB DISP_SOURCES "display/src/*.cpp")
# add_library(display STATIC ${DISP_SOURCES})
# Edit the 3 lines, according to your build of curses (ncurse or pdcurses)
# set(CURSES_INCLUDE_PATH "C:/Users/adity/libs/vcpkg/packages/pdcurses_x64-windows/include")
# set(CURSES_LIBRARY "C:/Users/adity/libs/vcpkg/packages/pdcurses_x64-windows/lib")
#target_include_directories(simulator PRIVATE curses/includes)
# Edit the previous 3 lines, according to your build of curses (ncurse or pdcurses)
# define a macro here "__DEBUG"
# include_directories(${CURSES_INCLUDE_PATH})
# find_package(Curses REQUIRED)
# target_link_libraries(display PUBLIC ${CURSES_LIBRARY}/pdcurses.lib)
