#include <vector>

#include "dbscan.h"
#include "original.h"
#include "point.h"

std::pair<std::vector<Point>, int> dbscan::cluster(std::vector<Point>& points)
{
    /** dbscan parameters */
    float epsilon = 3.317;
    const int MIN_POINTS = 4;

    /** cluster points and track number of clusters using original dbscan */
    auto sptr_points = std::make_shared<std::vector<Point>>(points);
    int numClusters = original::cluster(sptr_points, MIN_POINTS, epsilon);

    return { *sptr_points, numClusters };
}
