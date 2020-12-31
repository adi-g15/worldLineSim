cmake_minimum_required(VERSION 3.10.0)
project(World-Line-Simulator VERSION 0.271)

set(CMAKE_CXX_STANDARD 20)


#set(CMAKE_TOOLCHAIN_FILE "C:/Users/adity/libs/vcpkg/scripts/buildsystems/vcpkg.cmake")

file(GLOB SOURCES "src/*.cpp")
# later distribute display into a library, and link to it
file(GLOB DISP_SOURCES "display/src/*.cpp")

add_library(display STATIC ${DISP_SOURCES})

add_compile_definitions(GRAPH_MAT_NO_COORD)
add_executable(graph_test "graphMat/test.cxx")
add_executable(simulator main.cpp ${SOURCES})
target_link_libraries(simulator PRIVATE display)

include_directories(includes)
include_directories(.)

#add_library()

set(DEBUG_MODE true)

# Edit the 3 lines, according to your build of curses (ncurse or pdcurses)
set(CURSES_INCLUDE_PATH "C:/Users/adity/libs/vcpkg/packages/pdcurses_x64-windows/include")
set(CURSES_LIBRARY "C:/Users/adity/libs/vcpkg/packages/pdcurses_x64-windows/lib")
#target_include_directories(simulator PRIVATE curses/includes)
# Edit the previous 3 lines, according to your build of curses (ncurse or pdcurses)

include_directories(${CURSES_INCLUDE_PATH})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Curses REQUIRED)

link_libraries(Threads::Threads)
target_link_libraries(display PUBLIC ${CURSES_LIBRARY}/pdcurses.lib)

set(TEST_CPP "src/world.cpp"  "src/world_node.cpp"  "src/world_plot.cpp"  "src/snake.cpp" "src/path_finder.cpp")
add_executable(test "test.cxx"  ${SOURCES})
target_link_libraries(test PRIVATE display)

# define a macro here "__DEBUG"
