cmake_minimum_required(VERSION 3.10.0)
project(WorldLineSim
    DESCRIPTION "WorldLine Simulator"
    LANGUAGES CXX
    VERSION 0.271
    )
include(CTest)

# Later utilise features in C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/include")
    message(FATAL_ERROR "The nanogui dependency is missing!! "
            "You might not have cloned the repo with --recursive "
            "You will need to clone the dependencies too, use \`git submodule --init --recursive\`"
        )
endif()

add_compile_definitions(GRAPH_MAT_NO_COORD)
add_compile_definitions(DEBUG)

set(NANOGUI_BUILD_EXAMPLES  OFF CACHE BOOL "" FORCE)
set(NANOGUI_BUILD_SHARED    OFF CACHE BOOL "" FORCE)
set(NANOGUI_BUILD_PYTHON    OFF CACHE BOOL "" FORCE)
set(NANOGUI_INSTALL         OFF CACHE BOOL "" FORCE)

add_subdirectory(ext/quill)
add_subdirectory(ext/nanogui)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(includes)   # should also be available to display
include_directories(.)
include_directories(ext/graphMat/includes/)
include_directories(ext/utilpp/includes/)

file(GLOB SOURCES "src/Entities/*.cpp" "src/*.cpp")
file(GLOB DISP_SOURCES "display/src/*.cpp")

add_executable(simulator main.cpp ${SOURCES} ${DISP_SOURCES})

target_link_libraries(simulator PRIVATE Threads::Threads)
target_link_libraries(simulator PRIVATE quill::quill )
target_link_libraries(simulator PRIVATE nanogui)

# set(CMAKE_TOOLCHAIN_FILE "C:/Users/adity/libs/vcpkg/scripts/buildsystems/vcpkg.cmake")
# file(GLOB DISP_SOURCES "display/src/*.cpp")
# set(CURSES_INCLUDE_PATH "C:/Users/adity/libs/vcpkg/packages/pdcurses_x64-windows/include")
# set(CURSES_LIBRARY "C:/Users/adity/libs/vcpkg/packages/pdcurses_x64-windows/lib")
#target_include_directories(simulator PRIVATE curses/includes)
# include_directories(${CURSES_INCLUDE_PATH})
# find_package(Curses REQUIRED)
# target_link_libraries(display PUBLIC ${CURSES_LIBRARY}/pdcurses.lib)
