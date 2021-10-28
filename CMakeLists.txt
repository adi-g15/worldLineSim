cmake_minimum_required(VERSION 3.15)
project(WorldLineSim
    DESCRIPTION "WorldLine Simulator"
    LANGUAGES CXX
    VERSION 0.271
    )
include(CTest)

#cmake_policy(SET CMP0097 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# add CPM
include(cmake/CPM.cmake)

file(GLOB SOURCES "src/Entities/*.cpp" "src/*.cpp")
file(GLOB CUSTOM_ENTITIES_SRC "src/Entities/Custom/*.cpp" "src/Entities/Meta/Custom/*/*.cpp")
file(GLOB DISP_SOURCES "display/src/*.cpp")

add_executable(worldlinesim main.cpp ${SOURCES} ${DISP_SOURCES} ${CUSTOM_ENTITIES_SRC})
#add_executable(_test _test.cpp)
#target_link_libraries(_test PRIVATE "graphMat" "utilpp")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # -xc++-system-header iostream
    target_compile_options(worldlinesim PRIVATE -fmodules-ts)
else(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # TODO: libc++ mandatory with clang
    target_compile_options(worldlinesim PRIVATE -fmodules -stdlib=libc++
            -fbuiltin-module-map)
endif()

add_custom_command(TARGET worldlinesim PRE_BUILD
        COMMAND ${CMAKE_CXX_COMPILER} -fmodules-ts -std=c++20 -x c++-system-header any
	)

add_custom_command(TARGET worldlinesim POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy_directory
		   	${CMAKE_SOURCE_DIR}/db $<TARGET_FILE_DIR:worldlinesim>/db
	)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(GRAPH_MAT_NO_COORD)
else()
    add_compile_definitions(DEBUG)
endif()

if(CMAKE_GENERATOR STREQUAL "Ninja")
    message("Enabling color in Ninja color")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
endif()

include_directories("include" "modules" ".")

# graphMat & utilpp
CPMAddPackage("gh:adi-g15/graphMat@1.0.3")
CPMAddPackage("gh:adi-g15/utilpp@1.0.4")
target_link_libraries(worldlinesim PRIVATE "graphMat" "utilpp")

# spdlog
CPMAddPackage("gh:gabime/spdlog@1.8.5")

# On linux can pass this flag to use already installed glfw
# -D
# nanogui
#find_package(glfw3)
#if(glfw3_FOUND)
#    CPMAddPackage(
#	    NAME "nanogui"
#	    GITHUB_REPOSITORY "mitsuba-renderer/nanogui"
#	    GIT_TAG "abd03f651b2e8fd168c30a5301d5348712198776"
#	    OPTIONS "NANOGUI_BUILD_EXAMPLES OFF" "NANOGUI_BUILD_GLFW OFF" "NANOGUI_BUILD_SHARED OFF" "NANOGUI_BUILD_PYTHON OFF" "NANOGUI_INSTALL OFF"
#	    )
#    target_link_libraries(worldlinesim PRIVATE glfw)
#else()
CPMAddPackage(
	NAME "nanogui"
	GITHUB_REPOSITORY "mitsuba-renderer/nanogui"
	GIT_TAG "abd03f651b2e8fd168c30a5301d5348712198776"
	OPTIONS "NANOGUI_BUILD_EXAMPLES OFF" "NANOGUI_BUILD_SHARED OFF" "NANOGUI_BUILD_PYTHON OFF" "NANOGUI_INSTALL OFF"
	)
#endif()

# yaml-cpp
CPMAddPackage(
	GITHUB_REPOSITORY "jbeder/yaml-cpp"
	GIT_TAG yaml-cpp-0.7.0
	GIT_SUBMODULES ""
	OPTIONS "YAML_CPP_BUILD_TESTS OFF" "YAML_CPP_INSTALL OFF"
	)

target_link_libraries(worldlinesim PRIVATE nanogui spdlog::spdlog yaml-cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(worldlinesim PRIVATE Boost::headers)
add_library(arraydisksaver "modules/internal/storage/ArrayDiskSaver/src/impl.cpp")
target_include_directories(arraydisksaver PRIVATE "modules/internal/storage/ArrayDiskSaver/include")

add_executable(test_target "modules/internal/storage/ArrayDiskSaver/tests/array_file.cpp")
target_include_directories(test_target PRIVATE "modules/internal/storage/ArrayDiskSaver/include")
target_link_libraries(test_target PRIVATE arraydisksaver)

target_link_libraries(worldlinesim PRIVATE Threads::Threads)

# If you want, you can install 'worldlinesim' executable :D
install(TARGETS worldlinesim DESTINATION bin COMPONENT binaries)
