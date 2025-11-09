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
    // Struct to hold the aggregated results for one tree type
    struct AvgResult {
        std::string name;
        std::chrono::milliseconds total_build_time{0};
        std::chrono::microseconds total_query_time{0};
        int runs = 0;

        std::chrono::milliseconds avg_build_time() const {
            return (runs > 0) ? total_build_time / runs : std::chrono::milliseconds(0);
        }
        std::chrono::microseconds avg_query_time() const {
            return (runs > 0) ? total_query_time / runs : std::chrono::microseconds(0);
        }
    };

    // Runs a full suite of tests and prints a comparative summary
    void run_suite(const std::vector<Restaurant>& data);

    // Runs a benchmark comparing parsing times of raw (GeoJSON) vs binary formats
    void run_parser_benchmark(const std::string& geojson_path, const std::string& binary_path);

private:
    // Struct to define a single test case for the benchmark
    struct TestCase {
        double lat;
        double lon;
        std::size_t k;
        std::string description;
    };
};

} // namespace geo
