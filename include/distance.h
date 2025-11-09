#pragma once

#include <cmath>
#include "quadtree.h"
#include "kdtree.h"

namespace geo {
struct Distance {
    // Used only for final distance calculation on k-NN results (slow but accurate)
    static double haversine(const double lat1, const double lon1, const double lat2, const double lon2);
};
}