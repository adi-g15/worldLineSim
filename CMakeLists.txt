cmake_minimum_required(VERSION 3.15)
project(WorldLineSim
    DESCRIPTION "WorldLine Simulator"
    LANGUAGES CXX
    VERSION 0.271
    )
include(CTest)

#cmake_policy(SET CMP0097 NEW)

# Later utilise features in C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# add CPM
include(cmake/CPM.cmake)

file(GLOB SOURCES "src/Entities/*.cpp" "src/*.cpp")
file(GLOB CUSTOM_ENTITIES_SRC "src/Entities/Custom/*.cpp" "src/Entities/Meta/Custom/*/*.cpp")
file(GLOB DISP_SOURCES "display/src/*.cpp")

add_executable(worldlinesim main.cpp ${SOURCES} ${DISP_SOURCES} ${CUSTOM_ENTITIES_SRC})

add_custom_command(TARGET worldlinesim POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy_directory
		   	${CMAKE_SOURCE_DIR}/db $<TARGET_FILE_DIR:worldlinesim>/db
	)

if(CMAKE_BUILD_TYPE EQUAL "Release")
    add_compile_definitions(GRAPH_MAT_NO_COORD)
else()
    add_compile_definitions(DEBUG)
endif()

include_directories(includes)   # should also be available to display
include_directories(.)


# graphMat & utilpp
CPMAddPackage("gh:adi-g15/graphMat@1.0.1")
CPMAddPackage("gh:adi-g15/utilpp@1.0.3")
target_link_libraries(worldlinesim PRIVATE graphMat)
target_link_libraries(worldlinesim PRIVATE utilpp)

# spdlog
CPMAddPackage("gh:gabime/spdlog@1.8.5")

# nanogui
CPMAddPackage(
	NAME "nanogui"
	GITHUB_REPOSITORY "mitsuba-renderer/nanogui"
	GIT_TAG "9371d845cbdeb548ac8677656669044fd3a3f2a2"
	OPTIONS "NANOGUI_BUILD_EXAMPLES OFF" "NANOGUI_BUILD_SHARED OFF"	"NANOGUI_BUILD_PYTHON OFF" "NANOGUI_INSTALL OFF"
	)

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

add_library(arraydisksaver "modules/internal/ArrayDiskSaver/src/impl.cpp")
target_include_directories(arraydisksaver PRIVATE "modules/internal/ArrayDiskSaver/include")

add_executable(test_target "modules/internal/ArrayDiskSaver/tests/array_file.cpp")
target_include_directories(test_target PRIVATE "modules/internal/ArrayDiskSaver/include")
target_link_libraries(test_target PRIVATE arraydisksaver)

target_link_libraries(worldlinesim PRIVATE Threads::Threads)

# If you want, you can install 'worldlinesim' executable :D
install(TARGETS worldlinesim DESTINATION bin COMPONENT binaries)
