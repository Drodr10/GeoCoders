#include "parser.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace geo {

std::vector<Restaurant> GeoJSONParser::parse(const std::string& filepath) const {
    std::ifstream file("../data/data.bin"); // this might be dumb but it's the only way I could think of to check if the binary file exists
    if (!file.is_open())
        return GeoJSONParser::parseFromGeoJSON(filepath);
    else{
        file.close();
        return GeoJSONParser::deserializeFromBinary();
    }
}

std::vector<Restaurant> GeoJSONParser::parseFromGeoJSON(const std::string& filepath) const {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

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

        restaurants.push_back(restaurant);
    }

    file.close();

    GeoJSONParser::serializeToBinary(restaurants);

    return restaurants;
}

    void GeoJSONParser::serializeToBinary(const std::vector<Restaurant>& restaurants) const {
        std::ofstream file("../data/data.bin", std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for writing: ../data/data.bin");
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

        file.close();
    }

    std::vector<Restaurant> GeoJSONParser::deserializeFromBinary() const {
        std::ifstream file("../data/data.bin", std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for reading: ../data/data.bin\nThis should never happen, either something is terribly wrong, or you called the wrong function.");
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

        file.close();
        return restaurants;
    }



} // namespace geo
