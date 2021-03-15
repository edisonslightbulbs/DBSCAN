#include "dbscan.h"

extern const int FAIL;
extern const int PASS;

const float m_epsilon = 90;
const int MINIMUM_POINTS = 4;

#include "dbscan.h"

class optimized {

    std::vector<int>neighbourhood(Point t_point)
    {
        int index = 0;
        std::vector<int> neighbours;
        for (auto points : m_points) {
            if (t_point.distance(points) <= m_epsilon) {
                neighbours.push_back(index);
            }
            index++;
        }
        return neighbours;
    }

    int findNeighbors(Point t_point, int t_cluster)
    {
        std::vector<int> neighbours = neighbourhood(t_point);
        if (neighbours.size() < MINIMUM_POINTS) {
            t_point.m_cluster = NOISE;
            return FAIL;
        }

        int index = 0;
        int centroid = 0;
        for (auto neighbour : neighbours) {
            m_points.at(neighbour).m_cluster = t_cluster;
            if (m_points.at(neighbour) == t_point) {
                centroid = index;
            }
            ++index;
        }
        neighbours.erase(neighbours.begin() + centroid);

        for (std::vector<int>::size_type i = 0, n = neighbours.size(); i < n; ++i) {
            std::vector<int> nextNeighbours
                    = neighbourhood(m_points.at(neighbours[i]));

            if (nextNeighbours.size() >= MINIMUM_POINTS) {
                for (auto neighbour : nextNeighbours) {
                    if (m_points.at(neighbour).undefined()) {
                        neighbours.push_back(neighbour);
                        n = neighbours.size();
                        m_points.at(neighbour).m_cluster = t_cluster;
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
            if (findNeighbors(point, cluster) != FAIL) {
                cluster += 1;
            }
        }
    }
    m_clusterCount = cluster;
}

};