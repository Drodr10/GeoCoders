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
#include "restaurant.h"

#include <iostream>

int main(int /*argc*/, char** /*argv*/) {
    // TODO: implement main logic

    // This is how to use the parser
    geo::GeoJSONParser parser; // I could make it static if you want
    std::vector<geo::Restaurant> restaurants = parser.parse("../data/restaurants.geojson");

    std::cout << "Parsed " << restaurants.size() << " restaurants." << std::endl;

    return 0;
}
