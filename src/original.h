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

    std::vector<Point> cluster(std::vector<Point> points, const int& minPoints, const float& epsilon)
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
            point.m_cluster = cluster;

            /** remove current core from neighbours to expand */
            std::vector<Point> seedSet = neighbours;
            seedSet.erase(std::find(seedSet.begin(), seedSet.end(), point));
            for(int i = 0; i < seedSet.size(); i++){
                if (seedSet[i].m_cluster == NOISE) {
                    seedSet[i].m_cluster = cluster;
                }
                if (seedSet[i].m_cluster != UNDEFINED) {
                    continue;
                }
                seedSet[i].m_cluster = cluster;

                /** nearest neighbors */
                std::vector<Point> newNeighbours = query(points, seedSet[i], EPSILON);
                if (newNeighbours.size() >= minPoints) {
                    seedSet.insert(seedSet.end(), newNeighbours.begin(), newNeighbours.end());
                }
            }
        }
        // io::write(points, io::pwd() + "/build/dbscan.csv");

        return points;
    }
}