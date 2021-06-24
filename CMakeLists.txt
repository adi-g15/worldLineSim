cmake_minimum_required(VERSION 3.15)
project(WorldLineSim
    DESCRIPTION "WorldLine Simulator"
    LANGUAGES CXX
    VERSION 0.271
    )
include(CTest)

# Later utilise features in C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# add CPM
include(cmake/CPM.cmake)

# graphMat & utilpp
CPMAddPackage("gh:adi-g15/graphMat@1.0.1")
CPMAddPackage("gh:adi-g15/utilpp@1.0.1")

# spdlog
CPMAddPackage("gh:gabime/spdlog@1.8.5")

# nanogui
CPMAddPackage(
	NAME "nanogui"
	GITHUB_REPOSITORY "mitsuba-renderer/nanogui"
	GIT_TAG "0146a88b2214cd5c5c29e6dfa8d3d3d0e9ab6d9d"
	OPTIONS "NANOGUI_BUILD_EXAMPLES  OFF"	"NANOGUI_BUILD_SHARED    OFF"	"NANOGUI_BUILD_PYTHON    OFF"	"NANOGUI_INSTALL         OFF"
	)

if(CMAKE_BUILD_TYPE EQUAL "Release")
    add_compile_definitions(GRAPH_MAT_NO_COORD)
else()
    add_compile_definitions(DEBUG)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(includes)   # should also be available to display
include_directories(.)

file(GLOB SOURCES "src/Entities/*.cpp" "src/*.cpp")
file(GLOB CUSTOM_ENTITIES_SRC "src/Entities/Custom/*.cpp")
file(GLOB DISP_SOURCES "display/src/*.cpp")

add_executable(simulator main.cpp ${SOURCES} ${DISP_SOURCES} ${CUSTOM_ENTITIES_SRC})

target_link_libraries(simulator PRIVATE graphMat)
target_link_libraries(simulator PRIVATE utilpp)
target_link_libraries(simulator PRIVATE Threads::Threads)
target_link_libraries(simulator PRIVATE nanogui spdlog::spdlog)

