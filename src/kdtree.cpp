#include "kdtree.h"



namespace geo {

// Simple node: one point, axis, left/right children
struct KDTree::Node {
    Restaurant point;
    int axis; // 0 = split by latitude, 1 = split by longitude
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

std::vector<std::pair<double, Restaurant>>
geo::KDTree::knn(double latitude, double longitude, std::size_t k) const {
    return knnSearch(latitude, longitude, k);
}

// Playbook for std::sort, used to compare values.
static bool compareLat(const Restaurant& a, const Restaurant& b) {
    return a.latitude < b.latitude;
}

//Playbook for std::sort, used to compare values.
static bool compareLon(const Restaurant& a, const Restaurant& b) {
    return a.longitude < b.longitude;
}


KDTree::KDTree() = default;
KDTree::~KDTree() = default;

// Public build: copy points, then build recursively
void KDTree::build(const std::vector<Restaurant>& points) {

    std::vector<Restaurant> copy = points;
    root_ = buildNode(copy, 0, static_cast<int>(copy.size()), 0);
}


//build Node method, helps to build the subtree, by sorting the vector by either x or y, and then adding the middle value to the tree. Completed recursively.
std::unique_ptr<KDTree::Node> KDTree::buildNode(std::vector<Restaurant>& points, int start, int end, int depth) {
    if (start >= end) {
        return nullptr;
    }

    int axis = depth % 2;

    //sorts the vector based on the latitude or longitude
    if (axis == 0) {
        std::sort(points.begin() + start, points.begin() + end, compareLat);
    }
    else {
        std::sort(points.begin() + start, points.begin() + end, compareLon);
    }

    int mid = (start + end) / 2;
    std::unique_ptr<Node> node(new Node());

    node->axis = axis;
    node->point = points[mid];
    node->left = buildNode(points, start, mid, depth + 1);
    node->right = buildNode(points, mid + 1, end, depth + 1);

    return node;
}


// Public k-NN interface - used for searching, the start of the kNN search. Returns the closest k restaurants in a vector format
std::vector<std::pair<double, Restaurant>> KDTree::knnSearch(double latitude,
                                                       double longitude,
                                                       std::size_t k) const {
    std::vector<std::pair<double, Restaurant>> result; //vector of the restaurant nodes that need to be returned.

    if (!root_ || k == 0) {
        return result;
    }

    // Max-heap: top() has largest distance so far
    std::priority_queue<std::pair<double, const Restaurant*>> pq;

    //recursive knn search
    knnRecur(root_.get(), latitude, longitude, k, pq);

    // Convert heap to result vector with real (haversine) distances
    while (!pq.empty()) {
        const Restaurant* r = pq.top().second;
        double d = Distance::haversine(latitude, longitude, r->latitude, r->longitude);
        result.push_back(std::make_pair(d, *r));
        pq.pop();
    }

    std::reverse(result.begin(), result.end()); // sort the heap so that it is in order based on distance.
    return result;
}

// Recursive k-NN search
void KDTree::knnRecur(const Node* node,
                      double latitude,
                      double longitude,
                      std::size_t k,
                      std::priority_queue<std::pair<double, const Restaurant*>>& pq) const {
    if (node == nullptr) {
        return;
    }

    // 1. Visit this node's point - calculate distance to determine which direction we need to go.
    double d2 = distanceSquared(latitude, longitude,
                                node->point.latitude, node->point.longitude);

    //If size less than k, add. If size greater than k
    if (pq.size() < k) {
        pq.push(std::make_pair(d2, &node->point));
    }
    else if (d2 < pq.top().first) {
        pq.pop();
        pq.push(std::make_pair(d2, &node->point));
    }

    // 2. Decide which side to visit first
    double queryCoord; //coordinate of point that is given
    double nodeCoord; //coordinate of the node in question

    if (node->axis == 0) {
        queryCoord = latitude;
        nodeCoord = node->point.latitude;
    } else {
        queryCoord = longitude;
        nodeCoord = node->point.longitude;
    }

    const Node* firstChild;
    const Node* secondChild;

    if (queryCoord < nodeCoord) {
        firstChild = node->left.get(); //need to go smaller so go left
        secondChild = node->right.get();
    } else {
        firstChild = node->right.get(); //need to go higher so go right
        secondChild = node->left.get();
    }

    // 3. Search the closer side first
    knnRecur(firstChild, latitude, longitude, k, pq);

    // 4. Decide if we need to search the other side
    double diff = queryCoord - nodeCoord;
    double diff2 = diff * diff;

    //checks farther child.
    if (pq.size() < k || diff2 < pq.top().first) { //checks for whether we have our k restaurants, and secondly, whether the difference of the node is less than the farthest distance on the queue.
        knnRecur(secondChild, latitude, longitude, k, pq);
    }
}

// Simple squared Euclidean distance - square root not needed currently.
double KDTree::distanceSquared(double lat1, double lon1,
                               double lat2, double lon2) const {
    double dlat = lat1 - lat2;
    double dlon = lon1 - lon2;
    return dlat * dlat + dlon * dlon;
}

} // namespace geo
