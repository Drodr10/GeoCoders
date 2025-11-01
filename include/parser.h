#pragma once

#include <string>
#include <vector>

#include "restaurant.h"

namespace geo {

// Blueprint for a GeoJSON parser.
// Implementation might use a JSON library (probably nlohmann/json) to parse the GeoJSON file
// and extract point coordinates or polygon centroids into Restaurant structs.
class GeoJSONParser {
public:
    GeoJSONParser() = default;

    // Parse the given file path and return a vector of Restaurant objects.
    // Throws on I/O or parse error (to be defined during implementation).
    std::vector<Restaurant> parse(const std::string& filepath) const;
};

} // namespace geo
