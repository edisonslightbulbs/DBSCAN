#include "dbscan.h"

namespace original{
    const float EPSILON = 90;

    std::vector<Point> query(
            std::vector<Point> points, Point core, const float& epsilon)
    {
        /** neighboring points */
        std::vector<Point> neighbours;
        for (const auto& point : points) {
            if (core.distance(point) <= EPSILON) {
                neighbours.push_back(point);
            }
        }
        return neighbours;
    }

    std::pair<std::vector<Point>, int> cluster(std::vector<Point> points, const int& minPoints, const float& epsilon)
    {
/**
 * DBSCAN
 *   Original query-based algorithm:
 *   https://en.wikipedia.org/wiki/DBSCAN
 *   date: 2021-03-06 16:04
 */
        /** run counter */
        int cluster = 0;

        for (auto point : points) {
            if (point.m_cluster != UNDEFINED) {
                continue;
            }

            /** neighboring points */
            std::vector<Point> neighbours = query(points, point, EPSILON);
            if (neighbours.size() < minPoints) {
                point.m_cluster = NOISE;
                continue;
            }
            cluster = cluster + 1;
            /** label point */
            point.m_cluster = cluster;

            /** remove current core from neighbours to expand */
            std::cout << neighbours.size() << std::endl;
            neighbours.erase(std::find(neighbours.begin(), neighbours.end(), point));
            std::cout << neighbours.size() << std::endl;
            for (int i = 0, n = neighbours.size(); i < n; ++i) {
                // if (neighbours[i].m_cluster == NOISE) {
                //     neighbours[i].m_cluster = cluster;
                // }
                // if (neighbours[i].m_cluster != UNDEFINED) {
                //     continue;
                // }
                // neighbours[i].m_cluster = cluster;

                /** nearest neighbors */
                std::vector<Point> newNeighbours = query(points, neighbours[i], EPSILON);
                if (newNeighbours.size() >= minPoints) {
                    for (auto neighbour : newNeighbours) {
                        if (neighbour.undefined()) {
                            neighbours.push_back(neighbour);
                            n = neighbours.size();
                            neighbour.m_cluster = cluster;
                        }
                    }
                }
            }
        }
        // io::write(points, io::pwd() + "/build/dbscan.csv");

        return { points, cluster };
    }
}
