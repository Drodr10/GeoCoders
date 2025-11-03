#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "restaurant.h"

namespace geo {

class GeoJSONParser {
    std::vector<Restaurant> parseFromGeoJSON(const std::string& filepath) const;

    void serializeToBinary(const std::vector<Restaurant>& restaurants) const;
    std::vector<Restaurant> deserializeFromBinary() const;

public:
    GeoJSONParser() = default;

    std::vector<Restaurant> parse(const std::string& filepath) const;
};

} // namespace geo
