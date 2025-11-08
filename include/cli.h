#pragma once

#include "kdtree.h"
#include "quadtree.h"
#include <vector>
#include "restaurant.h"
#include "spatial_index.h"

namespace geo {

// CLI blueprint: parse user input and present options.
class CLI {
public:
    CLI(const KDTree& kdtree, const Quadtree& quadtree, const std::vector<Restaurant>& restaurants);

    // Run the command-line interface. (obviously)
    void run();

private:
    const KDTree& kdtree_;
    const Quadtree& quadtree_;
    const std::vector<Restaurant>& restaurants_;
    // helper methods for drawing menu/input parsing
    std::string prompt(const std::string& msg) const;
    void perform_knn_search(const SpatialIndex& index, const std::string& tree_name);
    void run_benchmark();
};

} // namespace geo
