cmake_minimum_required(VERSION 3.10.0)
project(World-Line-Simulator VERSION 0.1.5)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cpp")
add_executable(simulator main.cpp ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(simulator PRIVATE Threads::Threads)

target_include_directories(simulator PRIVATE includes)
target_include_directories(simulator PRIVATE .)
