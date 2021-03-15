#ifndef DBSCAN_H
#define DBSCAN_H

#include "point.h"
#include <vector>

namespace dbscan {

std::pair<std::vector<Point>, int> run(
    std::vector<Point>& points, const int& minPoints, const float& epsilon);
};
#endif /* DBSCAN_H */
