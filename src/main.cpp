#include "cli.h"
#include "parser.h"
#include "quadtree.h"
#include "kdtree.h"
#include "benchmark.h"

#include <iostream>
#include <chrono>

int main(int /*argc*/, char** /*argv*/) {
    geo::GeoJSONParser parser;
    geo::GeoJSONParser::Result result = parser.parse("../data/restaurants.geojson");

    if (result.restaurants.empty()) {
        std::cerr << "Failed to parse restaurants." << std::endl;
        return 1;
    }

    std::cout << "Loading restaurant data from file..." << std::endl;
    std::cout << "Successfully parsed and loaded " << result.restaurants.size() << " restaurants into memory." << std::endl;

    auto start_kd = std::chrono::high_resolution_clock::now();
    geo::KDTree kdtree;
    kdtree.build(result.restaurants);
    auto end_kd = std::chrono::high_resolution_clock::now();
    auto kd_build_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_kd - start_kd);

    std::cout << "Building k-d tree... Done. (Time: " << kd_build_time.count() << " ms)" << std::endl;

    auto start_quad = std::chrono::high_resolution_clock::now();
    geo::Quadtree quadtree(result.restaurants, result.min_lat, result.max_lat, result.min_long, result.max_long);
    quadtree.build(result.restaurants);
    auto end_quad = std::chrono::high_resolution_clock::now();
    auto quad_build_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_quad - start_quad);

    std::cout << "Building Quadtree... Done. (Time: " << quad_build_time.count() << " ms)" << std::endl;

    geo::CLI cli(kdtree, quadtree, result);
    cli.run();

    return 0;
}
