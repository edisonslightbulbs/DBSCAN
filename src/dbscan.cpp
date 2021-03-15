#include "dbscan.h"
#include "logger.h"
#include "modified.h"
#include "original.h"
#include "timer.h"

std::pair<std::vector<Point>, int> dbscan::run(
    std::vector<Point>& points, const int& minPoints, const float& epsilon)
{
    std::pair<std::vector<Point>, int> clustered;
    // {
    //     Timer timer;
    //     clustered = original::cluster(points, minPoints, epsilon);
    //     LOG(INFO) << timer.getDuration()
    //               << " ms: dbscan runtime (!unoptimized!) ";
    // }
    {
        Timer timer;
        modified dbscan(points);
        clustered = {dbscan.m_points, dbscan.m_clusterCount};
        LOG(INFO) << timer.getDuration()
                  << " ms: dbscan runtime (!unoptimized!) ";
    }
    return clustered;
}
