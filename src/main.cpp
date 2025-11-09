#include "cli.h"
#include "parser.h"
#include "quadtree.h"
#include "kdtree.h"
#include "benchmark.h"

#include <iostream>
#include <chrono>

int main(int /*argc*/, char** /*argv*/) {
    std::cout << "Parsing GeoJSON data..." << std::endl;
    geo::GeoJSONParser parser;
    auto result = parser.parse("../data/restaurants.geojson");

    if (result.restaurants.empty()) {
        std::cerr << "Failed to parse GeoJSON or no data found." << std::endl;
        return 1;
    }
    std::cout << "Parsed " << result.restaurants.size() << " restaurants." << std::endl;

    std::cout << "Building k-d tree..." << std::endl;
    geo::KDTree kdtree;
    auto start_kd = std::chrono::high_resolution_clock::now();
    kdtree.build(result.restaurants);
    auto end_kd = std::chrono::high_resolution_clock::now();
    auto kd_build_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_kd - start_kd);

    std::cout << "Building k-d tree... Done. (Time: " << kd_build_time.count() << " ms)" << std::endl;  
    
    geo::Quadtree quadtree(result.min_lat, result.max_lat, result.min_long, result.max_long);
    auto start_quad = std::chrono::high_resolution_clock::now();
    quadtree.build(result.restaurants);
    auto end_quad = std::chrono::high_resolution_clock::now();
    auto quad_build_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_quad - start_quad);

    std::cout << "Building Quadtree... Done. (Time: " << quad_build_time.count() << " ms)" << std::endl;

    geo::CLI cli(kdtree, quadtree, result);
    cli.run();

    return 0;
}
