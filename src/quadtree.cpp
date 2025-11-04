#include "quadtree.h"
#include <iostream> // Temporary for debugging
namespace geo {

const int NODE_CAP = 4;

struct Quadtree::Node {
    std::unique_ptr<Node> nw;
    std::unique_ptr<Node> ne;
    std::unique_ptr<Node> sw;
    std::unique_ptr<Node> se;
    std::vector<Restaurant> points;

    // Bounding box for the node
    double min_lat = 0.0;
    double max_lat = 0.0;
    double min_long = 0.0;
    double max_long = 0.0;
};

Quadtree::Quadtree(const std::vector<Restaurant>& points, double min_lat, double max_lat, double min_long, double max_long) {
    root_ = std::make_unique<Node>();
    root_->min_lat = min_lat;
    root_->max_lat = max_lat;
    root_->min_long = min_long;
    root_->max_long = max_long;

    build(points);
}

Quadtree::~Quadtree() = default;

void Quadtree::build(const std::vector<Restaurant>& points) {
    if(!root_)
        return;

    for (const auto& restaurant : points) {
        insert(restaurant, root_.get());
    }
}

void Quadtree::insert(const Restaurant& restaurant, Node* node) {
    Node* current = node;

    while(current) {
        if (!inBoundingBox(restaurant, *current)) {
            std::cerr << "Error: Point out of bounds!" << std::endl;
            return;
        }

        double mid_lat = (current->min_lat + current->max_lat) / 2;
        double mid_long = (current->min_long + current->max_long) / 2;

        if (!current->nw) {
            bool at_precision_limit = (mid_lat == current->min_lat) || (mid_lat == current->max_lat) ||
                                      (mid_long == current->min_long) || (mid_long == current->max_long);
            if (current->points.size() < NODE_CAP || at_precision_limit) {
                current->points.push_back(restaurant);
                return;
            } else {
                subdivide(current);
            }
        }

        if (restaurant.latitude >= mid_lat) {
            if (restaurant.longitude < mid_long)
                current = current->nw.get();
            else
                current = current->ne.get();
        }
        else {
            if (restaurant.longitude < mid_long)
                current = current->sw.get();
            else
                current = current->se.get();
        }
    }
}

void Quadtree::subdivide(Node* node) {
    if (node->nw) {
        std::cerr << "Error: Node already subdivided!" << std::endl;
        return;
    }

    double mid_lat = (node->min_lat + node->max_lat) / 2;
    double mid_long = (node->min_long + node->max_long) / 2;

    node->nw = std::make_unique<Node>();
    node->nw->min_lat = mid_lat;
    node->nw->max_lat = node->max_lat;
    node->nw->min_long = node->min_long;
    node->nw->max_long = mid_long;

    node->ne = std::make_unique<Node>();
    node->ne->min_lat = mid_lat;
    node->ne->max_lat = node->max_lat;
    node->ne->min_long = mid_long;
    node->ne->max_long = node->max_long;

    node->sw = std::make_unique<Node>();
    node->sw->min_lat = node->min_lat;
    node->sw->max_lat = mid_lat;
    node->sw->min_long = node->min_long;
    node->sw->max_long = mid_long;

    node->se = std::make_unique<Node>();
    node->se->min_lat = node->min_lat;
    node->se->max_lat = mid_lat;
    node->se->min_long = mid_long;
    node->se->max_long = node->max_long;

    std::vector<Restaurant> old_points = std::move(node->points);
    node->points.clear();
    for (const Restaurant& restaurant : old_points) {
        insert(restaurant, node);
    }
}

bool Quadtree::inBoundingBox(const Restaurant& restaurant, const Quadtree::Node& node) {
    return restaurant.latitude >= node.min_lat && restaurant.latitude <= node.max_lat &&
           restaurant.longitude >= node.min_long && restaurant.longitude <= node.max_long;
}

std::vector<std::pair<double, Restaurant>> Quadtree::knn(const double latitude, const double longitude, std::size_t k) const {
    std::priority_queue<std::pair<double, Restaurant>> pq;

    knn_recur(root_.get(), latitude, longitude, k, pq);
    std::vector<std::pair<double, Restaurant>> result;
    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        double dist = haversine(latitude, longitude, top.second.latitude, top.second.longitude);
        result.emplace_back(dist, top.second);
    }

    std::reverse(result.begin(), result.end()); // Closest first
    return result;
}

void Quadtree::knn_recur(const Node* node, const double latitude, const double longitude, std::size_t k,
                         std::priority_queue<std::pair<double, Restaurant>>& pq) const {
    if (!node){
        return;
    }

    if(pq.size() == k) {
        double min_dist_sq = minDistanceSquared(latitude, longitude, *node);

        // If the minimum distance to this node is greater than the farthest in pq, prune
        if (min_dist_sq >= pq.top().first)
            return;
    }

    for (const auto& restaurant : node->points) {
        // Square of Euclidean distance for comparison (avoids sqrt for efficiency)
        double dist_sq = (restaurant.latitude - latitude) * (restaurant.latitude - latitude) +
                      (restaurant.longitude - longitude) * (restaurant.longitude - longitude);
        if (pq.size() < k) {
            pq.emplace(dist_sq, restaurant);
        }
        else if (dist_sq < pq.top().first) {
            pq.pop();
            pq.emplace(dist_sq, restaurant);
        }
    }

    if (!node->nw){
        return;
    }

    std::vector<std::pair<double, const Quadtree::Node*>> children_dists = {
        {minDistanceSquared(latitude, longitude, *node->nw), node->nw.get()},
        {minDistanceSquared(latitude, longitude, *node->ne), node->ne.get()},
        {minDistanceSquared(latitude, longitude, *node->sw), node->sw.get()},
        {minDistanceSquared(latitude, longitude, *node->se), node->se.get()}
    };
    // Sort children by minimum distance to query point
    std::sort(children_dists.begin(), children_dists.end()); // O(1)

    // Traverse children in order of increasing distance (this helps with pruning)
    for (const auto& child : children_dists) {
        knn_recur(child.second, latitude, longitude, k, pq);
    }
}

double Quadtree::minDistanceSquared(const double latitude, const double longitude, const Quadtree::Node& node) const {
    double dlat = 0.0;
    if (latitude < node.min_lat)
        dlat = node.min_lat - latitude;
    else if (latitude > node.max_lat)
        dlat = latitude - node.max_lat;
    

    double dlon = 0.0;
    if (longitude < node.min_long)
        dlon = node.min_long - longitude;
    else if (longitude > node.max_long)
        dlon = longitude - node.max_long;

    return dlat * dlat + dlon * dlon;
}

double Quadtree::haversine(const double lat1, const double lon1, const double lat2, const double lon2) const {
    const double R = 3958.8;
    // Convert degrees to radians
    double phi1 = lat1 * M_PI / 180.0;
    double phi2 = lat2 * M_PI / 180.0;
    double delta_phi = (lat2 - lat1) * M_PI / 180.0;
    double delta_lambda = (lon2 - lon1) * M_PI / 180.0;

    double a = sin(delta_phi / 2) * sin(delta_phi / 2) +
               cos(phi1) * cos(phi2) *
               sin(delta_lambda / 2) * sin(delta_lambda / 2);
    // Using atan2 instead of asin since it's apparently more stable globally
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c; // Distance in miles (multiply by 5280 in cli if you want feet)
}

} // namespace geo