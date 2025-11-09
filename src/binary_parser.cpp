#include "binary_parser.h"
#include <fstream>
#include <iostream>

namespace geo {

// Helper to write a string to the binary file
void write_string(std::ofstream& out, const std::string& str) {
    size_t len = str.length();
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    out.write(str.c_str(), len);
}

// Helper to read a string from the binary file
std::string read_string(std::ifstream& in) {
    size_t len;
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    std::string str(len, '\0');
    in.read(&str[0], len);
    return str;
}

void write_restaurants_binary(const std::string& file_path, const std::vector<Restaurant>& restaurants) {
    std::ofstream out(file_path, std::ios::binary);
    if (!out) {
        std::cerr << "Error opening file for writing: " << file_path << std::endl;
        return;
    }

    size_t count = restaurants.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& r : restaurants) {
        write_string(out, r.name);
        out.write(reinterpret_cast<const char*>(&r.latitude), sizeof(r.latitude));
        out.write(reinterpret_cast<const char*>(&r.longitude), sizeof(r.longitude));
    }
}

std::vector<Restaurant> read_restaurants_binary(const std::string& file_path) {
    std::vector<Restaurant> restaurants;
    std::ifstream in(file_path, std::ios::binary);
    if (!in) {
        std::cerr << "Error opening file for reading: " << file_path << std::endl;
        return restaurants;
    }

    size_t count;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    restaurants.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        Restaurant r;
        r.name = read_string(in);
        in.read(reinterpret_cast<char*>(&r.latitude), sizeof(r.latitude));
        in.read(reinterpret_cast<char*>(&r.longitude), sizeof(r.longitude));
        restaurants.push_back(r);
    }

    return restaurants;
}

} // namespace geo
