// quadtree.cpp — implementation skeleton for Quadtree
// Provides stubs matching the header; full algorithm to be implemented later.

#include "quadtree.h"

namespace geo {

struct Quadtree::Node {
  // Placeholder: bounding box, child pointers, stored points, etc.
};

Quadtree::Quadtree() = default;

Quadtree::~Quadtree() = default;

void Quadtree::build(const std::vector<Restaurant>& points) {
  // TODO: build quadtree by inserting points and subdividing when necessary
  (void)points;
}

std::vector<std::pair<double, Restaurant>> Quadtree::knn(double latitude, double longitude, std::size_t k) const {
  // TODO: implement quadtree k-NN traversal
  (void)latitude; (void)longitude; (void)k;
  return {};
}

} // namespace geo
