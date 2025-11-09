#pragma once

#include "restaurant.h"
#include <vector>
#include <string>

namespace geo {

// Writes a vector of Restaurants to a binary file.
void write_restaurants_binary(const std::string& file_path, const std::vector<Restaurant>& restaurants);

// Reads a vector of Restaurants from a binary file.
std::vector<Restaurant> read_restaurants_binary(const std::string& file_path);

} // namespace geo
