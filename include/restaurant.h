#pragma once

#include <cstdint>
#include <string>

namespace geo {

struct Restaurant {
    std::int64_t osm_id; // OSM id
    std::string name;
    double longitude;
    double latitude;
    // Note: polygons will be collapsed to centroid before building indices.
};

} // namespace geo
