#include <memory>
#include <vector>

#include "dbscan.h"
#include "kdtree.h"
#include "original.h"
#include "point.h"

std::pair<std::vector<Point>, int> dbscan::cluster(
    std::vector<Point>& points, const float& E, const int& N)
{
    /** implementation 1) dbscan using original algorithm */
    auto sptr_points = std::make_shared<std::vector<Point>>(points);
    // int numClusters = original::cluster(sptr_points, MIN_POINTS, epsilon); //
    // <-naive naive implementation: original implementation of dbscan
    // translation of algorithm provided on wiki:
    // see@: https://en.wikipedia.org/wiki/DBSCAN
    //

    /** implementation 2) dbscan using kdtree (nanoflann's kdtree) */
    int numClusters = kdtree::cluster(sptr_points, N, E); // <-optimized
    // optimized implementation: based on nanoflann's optimized kdtree
    // implementations see@: https://github.com/jlblancoc/nanoflann
    //

    return { *sptr_points, numClusters };
}
