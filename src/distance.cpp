#include "distance.h"

namespace geo {

double Distance::haversine(const double lat1, const double lon1, const double lat2, const double lon2) {
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

}