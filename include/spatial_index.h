#pragma once

#include <vector>
#include <utility>
#include "restaurant.h"

namespace geo {

class SpatialIndex {
    // Build the index from a vector of restaurants. Called by constructor.
public:
    virtual void build(const std::vector<Restaurant>& points) = 0;
    virtual ~SpatialIndex() = default;

    // Find k nearest restaurants to the query point (lat, lon).
    // Returns a vector of pairs (distance, Restaurant).
    virtual std::vector<std::pair<double, Restaurant>> knn(double latitude, double longitude, std::size_t k) const = 0;
};

} // namespace geo
