#include "dbscan.h"

extern const int FAIL;
extern const int PASS;

const float m_epsilon = 90;
const int minimumPoints = 4;

#include "dbscan.h"

class optimized {

    std::vector<int>query(Point core)
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

    int dbscan(Point point, int cluster)
    {
        std::vector<int> neighbours = query(point);
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
        // std::cout << neighbours.size() << std::endl;
        // std::cout << neighbours.size() << std::endl;

        for (int i = 0, n = neighbours.size(); i < n; ++i) {
            std::vector<int> nextNeighbours
                    = query(m_points.at(neighbours[i]));

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

public:
    std::vector<Point> m_points;
    int m_clusterCount;

explicit optimized(std::vector<Point>& points)
        : m_points(points)
{
    int cluster = 0;
    for (auto point : m_points) {
        if (point.m_cluster == UNDEFINED) {
            if (dbscan(point, cluster) != FAIL) {
                cluster += 1;
            }
        }
    }
    m_clusterCount = cluster;
}

};
