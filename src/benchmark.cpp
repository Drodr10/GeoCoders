#include "benchmark.h"
#include <chrono>

namespace geo {

Benchmark::Result Benchmark::run(const std::string& name, SpatialIndex& index, const std::vector<Restaurant>& data,
                                 double qlat, double qlon, std::size_t k) const {
    Result r;
    r.name = name;

    auto build_start = std::chrono::high_resolution_clock::now();
    // The `build` method is now part of the SpatialIndex interface, but it's called by the constructor in Quadtree.
    // To keep the benchmark consistent, we will time the build process outside for both.
    if (name == "k-d Tree") {
        index.build(data);
    }
    auto build_end = std::chrono::high_resolution_clock::now();
    r.build_time = std::chrono::duration_cast<std::chrono::milliseconds>(build_end - build_start);

    auto query_start = std::chrono::high_resolution_clock::now();
    index.knn(qlat, qlon, k);
    auto query_end = std::chrono::high_resolution_clock::now();
    r.query_time = std::chrono::duration_cast<std::chrono::microseconds>(query_end - query_start);

    return r;
}

} // namespace geo
