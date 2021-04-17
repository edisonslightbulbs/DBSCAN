#ifndef DBSCAN_H
#define DBSCAN_H

#include "point.h"
#include <vector>

namespace dbscan {
std::pair<std::vector<Point>, int> original(std::vector<Point>& points);
}
#endif /* DBSCAN_H */
