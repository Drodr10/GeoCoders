// parser.cpp — implementation skeleton for GeoJSONParser
// This file contains the parse() stub. The real parser may use a JSON library
// (e.g., nlohmann/json) or a custom parser; decision deferred.

#include "parser.h"

namespace geo {

std::vector<Restaurant> GeoJSONParser::parse(const std::string& filepath) const {
    // TODO: implement parsing of GeoJSON file located at `filepath`.
    // At implementation time consider hiding heavy third-party includes here
    // (e.g., include <nlohmann/json.hpp> in this .cpp only).
    (void)filepath;
    return {};
}

} // namespace geo
