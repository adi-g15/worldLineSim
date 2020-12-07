cmake_minimum_required(VERSION 3.10.0)
project(World-Line-Simulator VERSION 0.271)

set(CMAKE_CXX_STANDARD 20)

include_directories(includes)
include_directories(.)

file(GLOB SOURCES "src/*.cpp")
# later distribute display into a library, and link to it
file(GLOB DISP_SOURCES "display/src/*.cpp")
add_executable(simulator main.cpp ${SOURCES} ${DISP_SOURCES} "test.cxx")

# Edit the 3 lines, according to your build of curses (ncurse or pdcurses)
set(CURSES_INCLUDE_PATH ./curses/includes)
set(CURSES_LIBRARY ./curses/curses.lib)
target_include_directories(simulator PRIVATE curses/includes)
# Edit the previous 3 lines, according to your build of curses (ncurse or pdcurses)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Curses REQUIRED)

link_libraries(Threads::Threads)
target_link_libraries(simulator PRIVATE curses)

add_executable(test "test.cxx")

# define a macro here "__DEBUG"
