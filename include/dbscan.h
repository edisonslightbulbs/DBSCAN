#ifndef DBSCAN_H
#define DBSCAN_H

#include "point.h"
#include <vector>

namespace dbscan {

/** dbscan::cluster
 *   Carries out density-based spatial clustering.
 *
 * @param points
 *   Unlabeled (un-clustered) set of points.
 *
 * @param epsilon
 *   Epsilon hyper-parameter.
 *
 * @param minPoints
 *   Number of points in epsilon neighourhood
 *
 * @return
 *   Labeled (clustered) set of Points.
 */
std::vector<std::vector<Point>> cluster(
    std::vector<Point>& points, const float& epsilon, const int& minPoints);
}
#endif /* DBSCAN_H */
