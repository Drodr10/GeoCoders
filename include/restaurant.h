#pragma once

#include <cstdint>
#include <string>

namespace geo {

struct Restaurant {
    std::int64_t osm_id; // OSM id
    std::string name;
    double longitude;
    double latitude;

    bool operator<(const Restaurant& other) const {
    return osm_id < other.osm_id;
    }
};

} // namespace geo
