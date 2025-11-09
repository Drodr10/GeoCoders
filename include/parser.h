#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "restaurant.h"

namespace geo {

class GeoJSONParser {
public:
    struct Result {
        std::vector<Restaurant> restaurants;
        double min_lat;
        double max_lat;
        double min_long;
        double max_long;
    };

    GeoJSONParser() = default;

    Result parse(const std::string& filepath) const;
    
    // Public in order to be benchmarked
    Result parseFromGeoJSON(const std::string& filepath) const;

    void serializeToBinary(const std::vector<Restaurant>& restaurants, double min_lat, double max_lat, double min_long, double max_long) const;
    Result deserializeFromBinary() const;
};

} // namespace geo
