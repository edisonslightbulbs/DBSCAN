#include "dbscan.h"
#include "logger.h"
#include "timer.h"

std::vector<Point> query(
    const std::vector<Point>& points, Point core, const float& epsilon)
{
    /** neighboring points */
    std::vector<Point> neighbours;
    for (const auto& point : points) {
        if (core.distance(point) <= 90) {
            neighbours.push_back(point);
        }
    }
    return neighbours;
}

std::vector<Point> DBSCAN::cluster(std::vector<Point> points, const int& minPoints, const float& epsilon)
{
    Timer timer;
/**
 * DBSCAN
 *   Original query-based algorithm:
 *   https://en.wikipedia.org/wiki/DBSCAN
 *   date: 2021-03-06 16:04
 */
    int cluster = -1;
    for (auto& point : points) {
        if (point.m_cluster != UNCLASSIFIED) {
            continue;
        }

        /** neighboring points */
        std::vector<Point> neighbours = query(points, point, 90);
        if (neighbours.size() < minPoints) {
            point.m_cluster = NOISE;
            continue;
        }
        cluster++;
        point.m_cluster = cluster;
        // seedset missing here !!!

        for (auto& n : neighbours) {
            if (n.m_cluster == NOISE) {
                n.m_cluster = cluster;
            }
            if (n.m_cluster != UNCLASSIFIED) {
                continue;
            }
            n.m_cluster = cluster;

            /** nearest neighbors */
            std::vector<Point> nn = query(points, n, 90);
            if (nn.size() >= minPoints) {
                neighbours.insert(neighbours.end(), nn.begin(), nn.end());
            }
        }
    }
    LOG(INFO) << timer.getDuration() << " ms: dbscan runtime (!unoptimized!) ";

    // io::write(points, io::pwd() + "/build/dbscan.csv");

    return points;
}
