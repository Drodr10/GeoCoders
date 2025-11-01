// benchmark.cpp — implementation skeleton for Benchmark
// Provides a minimal stub for the Benchmark::run() method. Replace with real timing logic
// when implementing the benchmarking harness.

#include "benchmark.h"

namespace geo {

Benchmark::Result Benchmark::run(const std::string& name, SpatialIndex& index, const std::vector<Restaurant>& data,
                                 double qlat, double qlon, std::size_t k) const {
    // TODO: implement measurement of build time and query time.
    (void)index; (void)data; (void)qlat; (void)qlon; (void)k;

    Result r;
    r.name = name;
    r.build_time = std::chrono::microseconds{0};
    r.query_time = std::chrono::microseconds{0};
    return r;
}

} // namespace geo
