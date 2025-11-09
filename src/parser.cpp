#include "parser.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace geo {

GeoJSONParser::Result GeoJSONParser::parse(const std::string& filepath) const {
    std::ifstream file("data/data.bin");
    if (!file.is_open())
        return GeoJSONParser::parseFromGeoJSON(filepath);
    else{
        file.close();
        return GeoJSONParser::deserializeFromBinary();
    }
}

GeoJSONParser::Result GeoJSONParser::parseFromGeoJSON(const std::string& filepath) const {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    double min_lat = 90.0;
    double max_lat = -90.0;
    double min_long = 180.0;
    double max_long = -180.0;

    json data = json::parse(file);

    std::vector<Restaurant> restaurants;
    for (const auto& feature : data["features"]) {
        Restaurant restaurant;
        //if properties doesn't have name, skip
        if (feature["properties"].find("name") == feature["properties"].end())
            continue;
            
        // [properties][@id] contains the OSM id in form "relation/1518177" or "node/9438518031", so
        // just extract the number after the slash
        restaurant.osm_id = static_cast<int64_t>(std::stoll(feature["properties"]["@id"].get<std::string>().substr(
            feature["properties"]["@id"].get<std::string>().find('/') + 1)));
        restaurant.name = feature["properties"]["name"];
        
        if (feature["geometry"].find("coordinates") != feature["geometry"].end()) {
            const auto& coordinates = feature["geometry"]["coordinates"];

            // Use the first coordinate for all geometry types for speed
            if (feature["geometry"]["type"] == "Point") {
                restaurant.longitude = coordinates[0].get<double>();
                restaurant.latitude = coordinates[1].get<double>();
            }
            else if (feature["geometry"]["type"] == "LineString") {
                restaurant.longitude = coordinates[0][0].get<double>();
                restaurant.latitude = coordinates[0][1].get<double>();
            }
            else if (feature["geometry"]["type"] == "Polygon") {
                restaurant.longitude = coordinates[0][0][0].get<double>();
                restaurant.latitude = coordinates[0][0][1].get<double>();
            }
            else if (feature["geometry"]["type"] == "MultiPolygon") {
                restaurant.longitude = coordinates[0][0][0][0].get<double>();
                restaurant.latitude = coordinates[0][0][0][1].get<double>();
            }
            else
                continue;
        }
        else
            continue;

        min_lat = std::min(min_lat, restaurant.latitude);
        max_lat = std::max(max_lat, restaurant.latitude);
        min_long = std::min(min_long, restaurant.longitude);
        max_long = std::max(max_long, restaurant.longitude);

        restaurants.push_back(restaurant);
    }

    file.close();

    GeoJSONParser::serializeToBinary(restaurants, min_lat, max_lat, min_long, max_long);
    Result result;
    result.restaurants = restaurants;
    result.min_lat = min_lat;
    result.max_lat = max_lat;
    result.min_long = min_long;
    result.max_long = max_long;
    return result;
}

void GeoJSONParser::serializeToBinary(const std::vector<Restaurant>& restaurants, double min_lat, double max_lat, double min_long, double max_long) const {
    std::ofstream file("data/data.bin", std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: data/data.bin");
    }

    size_t size = restaurants.size();
    // the cast basically converts the address of size to a pointer to a char (byte)
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (const auto& restaurant : restaurants) {
        file.write(reinterpret_cast<const char*>(&restaurant.osm_id), sizeof(restaurant.osm_id));

        size_t name_length = restaurant.name.size();
        file.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
        file.write(restaurant.name.c_str(), name_length);

        file.write(reinterpret_cast<const char*>(&restaurant.longitude), sizeof(restaurant.longitude));
        file.write(reinterpret_cast<const char*>(&restaurant.latitude), sizeof(restaurant.latitude));
    }

    file.write(reinterpret_cast<const char*>(&min_lat), sizeof(min_lat));
    file.write(reinterpret_cast<const char*>(&max_lat), sizeof(max_lat));
    file.write(reinterpret_cast<const char*>(&min_long), sizeof(min_long));
    file.write(reinterpret_cast<const char*>(&max_long), sizeof(max_long));

    file.close();
}

GeoJSONParser::Result GeoJSONParser::deserializeFromBinary() const {
    std::ifstream file("data/data.bin", std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading: data/data.bin\nThis should never happen, either something is terribly wrong, or you called the wrong function.");
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));

    // First allocate the memory (well, technically it wouldn't know how much to allocate since it can hold variable-length strings)
    std::vector<Restaurant> restaurants(size);
    for (auto& restaurant : restaurants) {
        file.read(reinterpret_cast<char*>(&restaurant.osm_id), sizeof(restaurant.osm_id));

        size_t name_length;
        file.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
        restaurant.name.resize(name_length);
        file.read(&restaurant.name[0], name_length);

        file.read(reinterpret_cast<char*>(&restaurant.longitude), sizeof(restaurant.longitude));
        file.read(reinterpret_cast<char*>(&restaurant.latitude), sizeof(restaurant.latitude));
    }

    double min_lat, max_lat, min_long, max_long;
    file.read(reinterpret_cast<char*>(&min_lat), sizeof(min_lat));
    file.read(reinterpret_cast<char*>(&max_lat), sizeof(max_lat));
    file.read(reinterpret_cast<char*>(&min_long), sizeof(min_long));
    file.read(reinterpret_cast<char*>(&max_long), sizeof(max_long));

    file.close();
    Result result;
    result.restaurants = restaurants;
    result.min_lat = min_lat;
    result.max_lat = max_lat;
    result.min_long = min_long;
    result.max_long = max_long;
    
    return result;
}

} // namespace geo