#pragma once

#include <vector>
#include <memory>
#include "restaurant.h"
#include "spatial_index.h"

namespace geo {

class KDTree : public SpatialIndex {
public:
    struct Node;

    KDTree();
    ~KDTree();

    // Build tree from points
    void build(const std::vector<Restaurant>& points) override;

    // k-nearest neighbors
    std::vector<std::pair<double, Restaurant>> knn(double latitude, double longitude, std::size_t k) const override;

private:
    // Look at quadtree.h for explanation about unique_ptr usage
    std::unique_ptr<Node> root_;
    // internal helpers will be implemented later
};

} // namespace geo
