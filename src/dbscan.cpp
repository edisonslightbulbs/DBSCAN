#include <memory>
#include <vector>

#include "dbscan.h"
#include "elbow.h"
#include "knn.h"
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

std::pair<std::vector<Point>, int> cluster(
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

std::vector<Point> largestDensity(std::pair<std::vector<Point>, int>& points)
{
    std::vector<std::vector<Point>> clusters(points.second);

    for (int i = 0; i <= points.second; ++i) {
        for (auto& point : points.first) {
            if (i == point.m_cluster) {
                clusters[i].push_back(point);
            }
        }
    }

    std::vector<Point> proposal;
    for (auto& cluster : clusters) {
        if (cluster.size() > proposal.size()) {
            proposal.clear();
            proposal = cluster;
        }
    }
    return proposal;
}

std::vector<Point> sample(std::vector<Point> points)
{
    const int SAMPLE_SIZE = 1000;

    /** find centroid */
    Point center = Point::centroid(points);

    /** compute distances from centroid for each point */
    for (auto& point : points) {
        float distance = point.distance(center);
        point.m_distance.second = distance;
    }

    /** sort points */
    Point::sort(points);

    /** sample edge of run points */
    std::vector<Point> sample
        = std::vector<Point>(points.end(), points.end() + SAMPLE_SIZE);

    return sample;
}

std::vector<Point> dbscan::run(std::vector<Point>& points)
{
    /** sample grown region */
    std::vector<Point> samplePoints = sample(points);

    /** query distances to 4th nearest neighbours */
    std::vector<float> knn4 = knn::compute(samplePoints);

    /** query epsilon  */
    // elbow::analyze(knn4);
    float epsilon = 114;
    const int MIN_POINTS = 4;

    /** cluster points and track number of clusters */
    auto sptr_points = std::make_shared<std::vector<Point>>(points);
    std::pair<std::vector<Point>, int> clusters;
    clusters = cluster(sptr_points, MIN_POINTS, epsilon);

    /** tabletop corresponds to the cluster with the largest density */
    std::vector<Point> tabletop
        = largestDensity(clusters); // <- tabletop interaction context

    return tabletop;
}
