#ifndef ORIGINAL_H
#define ORIGINAL_H

#include "point.h"
#include <vector>

extern const int FAIL;
extern const int PASS;

const float m_epsilon = 90;
const int minimumPoints = 4;

namespace optimized {

std::vector<int> queryRange(std::vector<Point> m_points, Point core)
{
    int index = 0;
    std::vector<int> neighbours;
    for (auto points : m_points) {
        if (core.distance(points) <= m_epsilon) {
            neighbours.push_back(index);
        }
        index++;
    }
    return neighbours;
}

int dbscan(std::vector<Point> m_points, Point point, int cluster)
{
    std::vector<int> neighbours = queryRange(m_points, point);
    if (neighbours.size() < minimumPoints) {
        point.m_cluster = NOISE;
        return FAIL;
    }

    int index = 0;
    int core = 0;
    for (auto neighbour : neighbours) {
        m_points.at(neighbour).m_cluster = cluster;
        if (m_points.at(neighbour) == point) {
            core = index;
        }
        ++index;
    }
    neighbours.erase(neighbours.begin() + core);

    for (std::vector<int>::size_type i = 0, n = neighbours.size(); i < n; ++i) {
        std::vector<int> nextNeighbours
            = queryRange(m_points, m_points.at(neighbours[i]));

        if (nextNeighbours.size() >= minimumPoints) {
            for (auto neighbour : nextNeighbours) {
                if (m_points.at(neighbour).undefined()) {
                    neighbours.push_back(neighbour);
                    n = neighbours.size();
                    m_points.at(neighbour).m_cluster = cluster;
                }
            }
        }
    }
    return PASS;
}

std::vector<Point> m_points;

std::pair<std::vector<Point> int> run(std::vector<Point>& points)
{
    m_points = points;
    int cluster = 0;
    for (auto point : m_points) {
        if (point.m_cluster == UNDEFINED) {
            if (dbscan(m_points, point, cluster) != FAIL) {
                cluster += 1;
            }
        }
    }
    return { m_points, cluster };
}
}
#endif /* ORIGINAL_H */
