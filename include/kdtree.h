#pragma once

#include <vector>
#include <memory>
#include <queue>
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

    // k-nearest neighbors - starting the search algorithm
    std::vector<std::pair<double, Restaurant>> knnSearch(double latitude, double longitude, std::size_t k) const override;

private:
    std::unique_ptr<Node> root_; //the first node in the tree.

    // Helper for building the tree
    std::unique_ptr<Node> buildNode(std::vector<Restaurant>& points,
                                    int start,
                                    int end,
                                    int depth);

    // Helper for k-NN search - searches the tree and updates the nearest restaraunts vector
    void knnRecur(const Node* node,
                  double latitude,
                  double longitude,
                  std::size_t k,
                  std::priority_queue<std::pair<double, const Restaurant*>>& pq) const;

    double distanceSquared(double lat1, double lon1,
                           double lat2, double lon2) const;

    // Used only for final distance calculation - basically converting to miles for the user.
    double haversine(double lat1, double lon1,
                     double lat2, double lon2) const;
};


} // namespace geo
