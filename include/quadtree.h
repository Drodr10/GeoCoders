#pragma once

#include <vector>
#include <memory>
#include "restaurant.h"
#include "spatial_index.h"

namespace geo {

class Quadtree : public SpatialIndex {
public:
    struct Node;

    Quadtree();
    ~Quadtree();

    // Build the quadtree from points
    void build(const std::vector<Restaurant>& points) override;

    // k-nearest neighbors using quadtree traversal
    std::vector<std::pair<double, Restaurant>> knn(double latitude, double longitude, std::size_t k) const override;

private:
    // Using unique_ptrs for the first time, i heard that it makes it much easier to manage memory
    // I can just forget about using delete root_ in the destructor!
    // Or just not use a destructor at all since unique_ptr will handle it for me, I think?
    std::unique_ptr<Node> root_;
    // internal helpers will be implemented later
};

} // namespace geo
