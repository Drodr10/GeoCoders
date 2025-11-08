#pragma once

#include <chrono>
#include <string>
#include <vector>
#include "restaurant.h"
#include "spatial_index.h"

namespace geo {

// Lightweight benchmarking harness blueprint.
class Benchmark {
public:
    struct Result {
        std::chrono::milliseconds build_time;
        std::chrono::microseconds query_time;
        // Additional metrics may be added
    };

    // Run benchmark for a particular index implementation.
    Result run(SpatialIndex& index, const std::vector<Restaurant>& data,
               double qlat, double qlon, std::size_t k) const;
};

} // namespace geo
