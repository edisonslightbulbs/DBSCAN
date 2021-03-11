#ifndef DBSCAN_H
#define DBSCAN_H

#include "point.h"
#include <vector>

class dbscan {

public:
    dbscan() = default;
    ~dbscan() = default;
    static std::vector<Point> cluster(
        std::vector<Point> points, const int& neighbourhoodSize);
};
#endif /* DBSCAN_H */
