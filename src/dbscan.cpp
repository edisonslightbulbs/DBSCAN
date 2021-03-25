#include <memory>
#include <vector>

#include "dbscan.h"
#include "point.h"

extern const int FAIL;
extern const int PASS;

std::vector<int> queryRange(std::shared_ptr<std::vector<Point>>& sptr_points,
    Point core, const float& epsilon)
{
    int index = 0;
    std::vector<int> neighbours;
    for (auto point : *sptr_points) {
        if (core.distance(point) <= epsilon) {
            neighbours.push_back(index);
        }
        index++;
    }
    return neighbours;
}

int scan(std::shared_ptr<std::vector<Point>>& sptr_points, Point point,
    int cluster, const int& minPoints, const float& epsilon)
{
    std::vector<int> neighbours = queryRange(sptr_points, point, epsilon);
    if (neighbours.size() < minPoints) {
        point.m_cluster = NOISE;
        return FAIL;
    }

    int index = 0;
    int core = 0;
    for (auto neighbour : neighbours) {
        sptr_points->at(neighbour).m_cluster = cluster;
        if (sptr_points->at(neighbour) == point) {
            core = index;
        }
        ++index;
    }
    neighbours.erase(neighbours.begin() + core);

    for (std::vector<int>::size_type i = 0, n = neighbours.size(); i < n; ++i) {
        std::vector<int> nextSet
            = queryRange(sptr_points, sptr_points->at(neighbours[i]), epsilon);

        if (nextSet.size() >= minPoints) {
            for (auto neighbour : nextSet) {
                if (sptr_points->at(neighbour).undefined()) {
                    neighbours.push_back(neighbour);
                    n = neighbours.size();
                    sptr_points->at(neighbour).m_cluster = cluster;
                }
            }
        }
    }
    return PASS;
}

std::pair<std::vector<Point>, int> run(
    std::shared_ptr<std::vector<Point>>& sptr_points, const int& minPoints,
    const float& epsilon)
{
    int cluster = 0;
    for (auto point : *sptr_points) {
        if (point.m_cluster == UNDEFINED) {
            if (scan(sptr_points, point, cluster, minPoints, epsilon) != FAIL) {
                cluster += 1;
            }
        }
    }

    return { *sptr_points, cluster };
}

std::pair<std::vector<Point>, int> dbscan::cluster(
    std::vector<Point>& points, const int& minPoints, const float& epsilon)
{
    auto sptr_points = std::make_shared<std::vector<Point>>(points);
    std::pair<std::vector<Point>, int> clustered;
    clustered = run(sptr_points, minPoints, epsilon);
    return clustered;
}
