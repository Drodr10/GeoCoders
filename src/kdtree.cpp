// kdtree.cpp — implementation skeleton for KDTree
// This file provides function stubs and minimal definitions. Implementations are TODO.

#include "kdtree.h"

namespace geo {

struct KDTree::Node {
  // Placeholder for node fields (split axis, point, children)
};

KDTree::KDTree() = default;

KDTree::~KDTree() = default;

void KDTree::build(const std::vector<Restaurant>& points) {
  // TODO: implement KD-tree construction (median split, recursive)
  (void)points;
}

std::vector<std::pair<double, Restaurant>> KDTree::knn(double latitude, double longitude, std::size_t k) const {
  // TODO: implement k-NN search using a max-heap of size k
  (void)latitude; (void)longitude; (void)k;
  return {};
}

} // namespace geo
