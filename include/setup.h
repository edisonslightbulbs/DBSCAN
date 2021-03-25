#ifndef SCAN_H
#define SCAN_H

#include <vector>
#include "point.h"

namespace setup {
std::pair<std::vector<Point>, int> density(std::vector<Point>& points);
}
#endif /* SCAN_H */
