#pragma once

#include <vector>
#include <utility>
#include "restaurant.h"

namespace geo {

// Abstract interface for spatial indices used by GeoQuery.
class SpatialIndex {
public:
// If you know java, this is just an interface. Subclasses must implement the methods.
    virtual ~SpatialIndex() = default; 

    // Build the index from a vector of restaurants.
    virtual void build(const std::vector<Restaurant>& points) = 0;

    // Find k nearest restaurants to the query point (lat, lon).
    // Returns a vector of pairs (distance, Restaurant). Distance uses Euclidean or haversine.
    virtual std::vector<std::pair<double, Restaurant>> knn(double latitude, double longitude, std::size_t k) const = 0;
};

} // namespace geo
