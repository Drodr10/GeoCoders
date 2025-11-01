cmake_minimum_required(VERSION 3.22)
project(GeoQuery)

set(CMAKE_CXX_STANDARD 17)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# if including json/hpp from nlohmann
# Include(FetchContent)
# FetchContent_Declare(
#   nlohmann_json
#   GIT_REPOSITORY https://github.com/nlohmann/json.git
#   GIT_TAG v3.11.2
# )
# FetchContent_MakeAvailable(nlohmann_json)

add_executable(geoquery src/main.cpp src/kdtree.cpp src/quadtree.cpp src/parser.cpp src/benchmark.cpp src/cli.cpp)
target_include_directories(geoquery PRIVATE ${PROJECT_SOURCE_DIR}/include)
