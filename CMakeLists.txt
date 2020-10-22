cmake_minimum_required(VERSION 3.10.0)
project(World-Line-Simulator VERSION 0.1.5)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cpp")
add_executable(simulator main.cpp ${SOURCES})
add_executable(test graphMat/test.cxx)

# Edit the 3 lines, according to your build of curses (ncurse or pdcurses)
set(CURSES_INCLUDE_PATH ./curses/includes)
set(CURSES_LIBRARY ./curses/pdcurses.lib)
target_include_directories(simulator PRIVATE curses/includes)
# Edit the previous 3 lines, according to your build of curses (ncurse or pdcurses)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Curses REQUIRED)

target_link_libraries(simulator PRIVATE Threads::Threads)

target_include_directories(simulator PRIVATE includes)
target_include_directories(simulator PRIVATE .)
target_include_directories(test PRIVATE .)
