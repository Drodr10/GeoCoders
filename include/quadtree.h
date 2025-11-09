#pragma once

#include <vector>
#include <memory>
#include <queue>
#include <algorithm>

#include "distance.h"
#include "restaurant.h"
#include "spatial_index.h"

namespace geo {

class Quadtree : public SpatialIndex {
public:
    struct Node;

    Quadtree(double min_lat, double max_lat, double min_long, double max_long);

    ~Quadtree();

    // k-nearest neighbors using quadtree traversal
    std::vector<std::pair<double, Restaurant>> knn(double latitude, double longitude, std::size_t k) const override;
    void build(const std::vector<Restaurant>& points) override;


private:
    std::unique_ptr<Node> root_;

    void insert(const Restaurant &restaurant, Node* node);
    void subdivide(Node* node);

    bool inBoundingBox(const Restaurant& restaurant, const Node& node);

    void knn_recur(const Node* node, const double latitude, const double longitude, std::size_t k,
                   std::priority_queue<std::pair<double, Restaurant>>& pq) const;

    double minDistanceSquared(const double latitude, const double longitude, const Quadtree::Node& node) const;
};

} // namespace geo
