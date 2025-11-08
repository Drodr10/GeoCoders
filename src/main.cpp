/*
  Intended responsibilities for `main`:
  - parse command-line args (optional file path)
  - call GeoJSONParser::parse to load data
  - build KDTree and Quadtree (measure build times)
  - present CLI menu (use CLI class)
  - run benchmarks and print results
*/

/**
 * I'm learning a lot about C++
 * Did you know that:
 * int main(int argc, char** argv) { return 0; }
 * int main(int, char**) { return 0; }
 * and the one below
 * are all valid ways to define the main function in C++ if using parameters?
 */

#include "parser.h"
#include "quadtree.h"
#include "kdtree.h"

#include <iostream>

int main(int /*argc*/, char** /*argv*/) {
    // TODO: implement main logic

    // This is how to use the parser
    geo::GeoJSONParser parser; // I could make it static if you want
    geo::GeoJSONParser::Result result = parser.parse("../data/restaurants.geojson");

    if (result.restaurants.size() > 0) {
        std::cout << "Parsed " << result.restaurants.size() << " restaurants." << std::endl;
    } else {
        std::cerr << "Failed to parse restaurants." << std::endl;
    }

    geo::Quadtree quadtree(result.restaurants, result.min_lat, result.max_lat, result.min_long, result.max_long);
    std::cout << "Quadtree built." << std::endl;
    auto knn_results = quadtree.knn(38.578584, -121.548289, 5); // Example: query 5 nearest neighbors to Sacramento coordinates
    std::cout << "5 nearest restaurants to (38.578584, -121.548289):" << std::endl;
    for (const auto& [dist, restaurant] : knn_results) {
        std::cout << "Restaurant: " << restaurant.name << ", Distance: " << dist << " miles" << std::endl;
    }

    //KD tree test
    geo::KDTree kdtree;
    kdtree.build(result.restaurants);
    std::cout << "KDTree built." << std::endl;

    auto kd_knn_results = kdtree.knnSearch(38.578584, -121.548289, 5);
    std::cout << "5 nearest restaurants to (38.578584, -121.548289):" << std::endl;
    for (const auto& pair : kd_knn_results) {
        const double dist = pair.first;
        const geo::Restaurant& restaurant = pair.second;
        std::cout << "Restaurant: " << restaurant.name
                  << ", Distance: " << dist << " miles" << std::endl;
    }


    return 0;
}
