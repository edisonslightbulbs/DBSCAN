#include <memory>

#include "dbscan.h"
#include "logger.h"
#include "original.h"
#include "timer.h"

std::pair<std::vector<Point>, int> dbscan::cluster(
    std::vector<Point>& points, const int& minPoints, const float& epsilon)
{
    Timer timer;

    auto sptr_points = std::make_shared<std::vector<Point>>(points);

    std::pair<std::vector<Point>, int> clustered;
    clustered = original::run(sptr_points, minPoints, epsilon);
    LOG(INFO) << timer.getDuration() << " ms: dbscan runtime (!unoptimized!)";
    return clustered;
}
