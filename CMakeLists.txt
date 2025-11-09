cmake_minimum_required(VERSION 3.22)
project(GeoQuery)

set(CMAKE_CXX_STANDARD 17)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(geoquery src/main.cpp src/kdtree.cpp src/quadtree.cpp src/parser.cpp src/benchmark.cpp src/cli.cpp src/distance.cpp)

target_link_libraries(geoquery PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(geoquery PRIVATE ${PROJECT_SOURCE_DIR}/include)
