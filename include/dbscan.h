#ifndef DBSCAN_H
#define DBSCAN_H

#include "point.h"
#include <vector>

namespace dbscan {
std::vector<Point> run(std::vector<Point>& points);
};
#endif /* DBSCAN_H */
