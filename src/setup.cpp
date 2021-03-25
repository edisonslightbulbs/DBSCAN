#include "setup.h"
#include "dbscan.h"
#include "knn.h"
#include "elbow.h"

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
        = std::vector<Point>(points.end(), points.end() + SAMPLE_SIZE );

    return sample;
}

std::pair<std::vector<Point>, int> setup::density(std::vector<Point>& points)
{
    /** sample grown region */
    std::vector<Point> samplePoints = sample(points);

    /** accumulate distances to 4th nearest neighbours */
    std::vector<float> knn4 = knn::compute(samplePoints);

    /** find epsilon query size */
    elbow::analyze(knn4);
    float epsilon = 10;

    /** run: return clustered points and number of clusters */
    const int MIN_POINTS = 4;
    std::pair<std::vector<Point>, int> clusters
        = dbscan::cluster(points, MIN_POINTS, epsilon);

    /** tabletop corresponds to the cluster with the largest density */
    std::vector<Point> tabletop = largestDensity(clusters); // <- tabletop interaction context

    return clusters;
}