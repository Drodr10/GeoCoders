#include "benchmark.h"
#include "kdtree.h"
#include "quadtree.h"
#include "parser.h"
#include <iostream>
#include <vector>
#include <iomanip>

namespace geo {

void Benchmark::run_suite(const std::vector<Restaurant>& data) {
    if (data.empty()) {
        std::cout << "No data to benchmark." << std::endl;
        return;
    }

    // Pre-calculate the bounding box for the entire dataset
    double min_lat = data[0].latitude;
    double max_lat = data[0].latitude;
    double min_lon = data[0].longitude;
    double max_lon = data[0].longitude;
    for (size_t i = 1; i < data.size(); ++i) {
        min_lat = std::min(min_lat, data[i].latitude);
        max_lat = std::max(max_lat, data[i].latitude);
        min_lon = std::min(min_lon, data[i].longitude);
        max_lon = std::max(max_lon, data[i].longitude);
    }

    // Define a suite of test cases
    const std::vector<TestCase> test_cases = {
        {34.0522, -118.2437, 5, "Dense Urban (Downtown LA, k=5)"},
        {34.0522, -118.2437, 50, "Dense Urban (Downtown LA, k=50)"},
        {34.1529, -118.4452, 10, "Suburban (Sherman Oaks, k=10)"},
        {33.7701, -118.1937, 20, "Coastal (Long Beach, k=20)"},
        {34.2383, -118.5233, 100, "Large Query (Northridge, k=100)"}
    };

    AvgResult kd_results{"k-d Tree"};
    AvgResult quad_results{"Quadtree"};

    std::cout << "Running benchmark suite with " << test_cases.size() << " test cases..." << std::endl;

    for (const auto& test : test_cases) {
        std::cout << "  Running test: " << test.description << "..." << std::endl;

        // Benchmark k-d Tree
        KDTree kd_tree;
        auto build_start_kd = std::chrono::high_resolution_clock::now();
        kd_tree.build(data);
        auto build_end_kd = std::chrono::high_resolution_clock::now();
        kd_results.total_build_time += std::chrono::duration_cast<std::chrono::milliseconds>(build_end_kd - build_start_kd);

        auto query_start_kd = std::chrono::high_resolution_clock::now();
        kd_tree.knn(test.lat, test.lon, test.k);
        auto query_end_kd = std::chrono::high_resolution_clock::now();
        kd_results.total_query_time += std::chrono::duration_cast<std::chrono::microseconds>(query_end_kd - query_start_kd);
        kd_results.runs++;

        // Benchmark Quadtree
        Quadtree quad_tree(min_lat, max_lat, min_lon, max_lon);
        auto build_start_quad = std::chrono::high_resolution_clock::now();
        quad_tree.build(data);
        auto build_end_quad = std::chrono::high_resolution_clock::now();
        quad_results.total_build_time += std::chrono::duration_cast<std::chrono::milliseconds>(build_end_quad - build_start_quad);

        auto query_start_quad = std::chrono::high_resolution_clock::now();
        quad_tree.knn(test.lat, test.lon, test.k);
        auto query_end_quad = std::chrono::high_resolution_clock::now();
        quad_results.total_query_time += std::chrono::duration_cast<std::chrono::microseconds>(query_end_quad - query_start_quad);
        quad_results.runs++;
    }

    std::cout << "\n--- Benchmark Summary ---\n" << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    // Print average results
    std::cout << "k-d Tree Averages:" << std::endl;
    std::cout << "  Avg. Build Time: " << kd_results.avg_build_time().count() << " ms" << std::endl;
    std::cout << "  Avg. Query Time: " << kd_results.avg_query_time().count() << " μs" << std::endl;

    std::cout << "\nQuadtree Averages:" << std::endl;
    std::cout << "  Avg. Build Time: " << quad_results.avg_build_time().count() << " ms" << std::endl;
    std::cout << "  Avg. Query Time: " << quad_results.avg_query_time().count() << " μs" << std::endl;

    // Calculate and print comparative analysis
    double build_factor = 0.0;
    double query_factor = 0.0;

    std::cout << "\n--- Comparative Analysis ---\n" << std::endl;

    // Build time comparison
    if (kd_results.avg_build_time() > quad_results.avg_build_time()) {
        if (quad_results.avg_build_time().count() > 0) {
            build_factor = static_cast<double>(kd_results.avg_build_time().count()) / quad_results.avg_build_time().count();
            std::cout << "Quadtree is " << build_factor << " times faster at building." << std::endl;
        }
    } else {
        if (kd_results.avg_build_time().count() > 0) {
            build_factor = static_cast<double>(quad_results.avg_build_time().count()) / kd_results.avg_build_time().count();
            std::cout << "k-d Tree is " << build_factor << " times faster at building." << std::endl;
        }
    }

    // Query time comparison
    if (kd_results.avg_query_time() > quad_results.avg_query_time()) {
        if (quad_results.avg_query_time().count() > 0) {
            query_factor = static_cast<double>(kd_results.avg_query_time().count()) / quad_results.avg_query_time().count();
            std::cout << "Quadtree is " << query_factor << " times faster at querying." << std::endl;
        }
    } else {
        if (kd_results.avg_query_time().count() > 0) {
            query_factor = static_cast<double>(quad_results.avg_query_time().count()) / kd_results.avg_query_time().count();
            std::cout << "k-d Tree is " << query_factor << " times faster at querying." << std::endl;
        }
    }
}

void Benchmark::run_parser_benchmark(const std::string& geojson_path) {
    std::cout << "\n--- Parser Benchmark ---\n" << std::endl;

    // Time GeoJSON parsing
    auto start_geojson = std::chrono::high_resolution_clock::now();
    GeoJSONParser geojson_parser;
    auto geojson_data = geojson_parser.parseFromGeoJSON(geojson_path);
    auto end_geojson = std::chrono::high_resolution_clock::now();
    auto geojson_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_geojson - start_geojson);

    std::cout << "GeoJSON Parsing Time: " << geojson_time.count() << " ms" << std::endl;

    // Time Binary parsing
    auto start_binary = std::chrono::high_resolution_clock::now();
    auto geojson_binary_data = geojson_parser.deserializeFromBinary();
    auto end_binary = std::chrono::high_resolution_clock::now();
    auto binary_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_binary - start_binary);

    std::cout << "Binary Parsing Time: " << binary_time.count() << " ms" << std::endl;

    // Comparative analysis
    if (geojson_time > binary_time) {
        if (binary_time.count() > 0) {
            double factor = static_cast<double>(geojson_time.count()) / binary_time.count();
            std::cout << "Binary parsing is " << std::fixed << std::setprecision(2) << factor << " times faster." << std::endl;
        }
    } else {
        if (geojson_time.count() > 0) {
            double factor = static_cast<double>(binary_time.count()) / geojson_time.count();
            std::cout << "GeoJSON parsing is " << std::fixed << std::setprecision(2) << factor << " times faster." << std::endl;
        }
    }
}

} // namespace geo
