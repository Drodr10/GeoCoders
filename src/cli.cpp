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
    std::cout << "\nRunning performance comparison benchmark suite..." << std::endl;
    benchmark.run_suite(result_.restaurants);
    benchmark.run_parser_benchmark("../data/restaurants.geojson", "../data/restaurants.bin");
}

std::string CLI::prompt(const std::string& msg) const {
    std::cout << msg;
    std::string input;
    std::cin >> input;
    return input;
}

} // namespace geo
