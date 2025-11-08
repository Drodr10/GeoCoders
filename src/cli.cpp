#include "cli.h"
#include "benchmark.h"
#include <iostream>
#include <limits>

namespace geo {

CLI::CLI(const KDTree& kdtree, const Quadtree& quadtree, const geo::GeoJSONParser::Result result)
    : kdtree_(kdtree), quadtree_(quadtree), result_(result) {}

void CLI::run() {
    while (true) {
        std::cout << "\nPlease select an option:\n"
                  << "1. Find nearest restaurants using k-d Tree\n"
                  << "2. Find nearest restaurants using Quadtree\n"
                  << "3. Run performance comparison benchmark\n"
                  << "4. Exit\n"
                  << "> Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                perform_knn_search(kdtree_, "k-d Tree");
                break;
            case 2:
                perform_knn_search(quadtree_, "Quadtree");
                break;
            case 3:
                run_benchmark();
                break;
            case 4:
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

void CLI::perform_knn_search(const SpatialIndex& index, const std::string& tree_name) {
    double lat, lon;
    int k;

    std::cout << "Enter latitude: ";
    std::cin >> lat;
    std::cout << "Enter longitude: ";
    std::cin >> lon;
    std::cout << "Enter number of restaurants to find (k): ";
    std::cin >> k;

    if (std::cin.fail() || k <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input for k." << std::endl;
        return;
    }

    std::cout << "\nSearching with " << tree_name << "..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();
    auto results = index.knn(lat, lon, k);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto query_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "(Time: " << query_time.count() << " microseconds)" << std::endl;
    std::cout << "The " << k << " closest restaurants are:" << std::endl;

    int rank = 1;
    for (const auto& pair : results) {
        std::cout << rank++ << ". " << pair.second.name << " (Distance: " << pair.first << " miles)" << std::endl;
    }
}

void CLI::run_benchmark() {
    Benchmark benchmark;
    double lat = 33.9313;
    double lon = -118.3961;
    int k = 5;

    std::cout << "\nRunning performance comparison benchmark..." << std::endl;
    
    // Non-const copies needed for benchmark
    KDTree kd_copy;
    Quadtree quad_copy(result_.restaurants, result_.min_lat, result_.max_lat, result_.min_long, result_.max_long);


    Benchmark::Result kd_result = benchmark.run(kd_copy, result_.restaurants, lat, lon, k);
    Benchmark::Result quad_result = benchmark.run(quad_copy, result_.restaurants, lat, lon, k);

    std::cout << "Benchmark Results (" << k << "-NN query at " << lat << ", " << lon << "):" << std::endl;
    std::cout << "  k-d Tree:" << std::endl;
    std::cout << "    Build Time: " << kd_result.build_time.count() << " ms" << std::endl;
    std::cout << "    Query Time: " << kd_result.query_time.count() << " μs" << std::endl;
    std::cout << "  Quadtree:" << std::endl;
    std::cout << "    Build Time: " << quad_result.build_time.count() << " ms" << std::endl;
    std::cout << "    Query Time: " << quad_result.query_time.count() << " μs" << std::endl;
}

std::string CLI::prompt(const std::string& msg) const {
    std::cout << msg;
    std::string input;
    std::cin >> input;
    return input;
}

} // namespace geo
