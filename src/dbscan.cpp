#include <iostream>

#include "dbscan.h"
#include "knn.h"
#include "logger.h"
#include "timer.h"

// std::vector<Point> query(
//     const std::vector<Point>& points, Point core, const float& EPSILON)
// {
//     /** neighboring points */
//     std::vector<Point> neighbours;
//     for (const auto& point : points) {
//         if (core.distance(point) < EPSILON) {
//             neighbours.push_back(point);
//         }
//     }
//     return neighbours;
// }
//
// float elbow(std::vector<float> distances)
// {
//     float vec1;
//     float vec2;
//     std::vector<float> angles;
//
//     for (int i = 1; i < distances.size() - 1; i++) {
//         vec1 = distances[i] - distances[i - 1];
//         vec2 = distances[i + 1] - distances[i];
//         float angle = std::atan2(vec1, vec2);
//         angles.push_back(angle);
//     }
//     float maxDiff = 0;
//     for (int i = 1; i < angles.size() - 1; i++) {
//         float d1 = angles[i - 1] - angles[i];
//         float d2 = angles[i + 1] - angles[i];
//         float diff = (d1 + d2);
//         if (diff > maxDiff) {
//             maxDiff = std::abs(angles[i]);
//         }
//     }
//     std::cout << maxDiff << std::endl;
//     return maxDiff;
// }
//
// std::vector<Point> dbscan::cluster(
//     std::vector<Point> points, const int& neighbourhoodSize)
// {
//     Timer timer;
//
//     /** compute knn where k = 4 and estimate epsilon */
//     std::vector<float> kdist = knn::compute(points);
//     const float EPSILON = elbow(kdist);
//
//     /** cluster counter / label */
//     int cluster = -1;
//
//     /**
//      * DBSCAN
//      *   Original query-based algorithm:
//      *   https://en.wikipedia.org/wiki/DBSCAN
//      *   date: 2021-03-06 16:04
//      */
//     for (auto& point : points) {
//         if (point.m_cluster != UNCLASSIFIED) {
//             continue;
//         }
//
//         /** neighboring points */
//         std::vector<Point> neighbours = query(points, point, EPSILON);
//         if (neighbours.size() < neighbourhoodSize) {
//             point.m_cluster = NOISE;
//             continue;
//         }
//         cluster++;
//         point.m_cluster = cluster;
//
//         for (auto& n : neighbours) {
//             if (n.m_cluster == NOISE) {
//                 n.m_cluster = cluster;
//             }
//             if (n.m_cluster != UNCLASSIFIED) {
//                 continue;
//             }
//             n.m_cluster = cluster;
//
//             /** nearest neighbors */
//             std::vector<Point> nn = query(points, n, EPSILON);
//             if (nn.size() >= neighbourhoodSize) {
//                 neighbours.insert(neighbours.end(), nn.begin(), nn.end());
//             }
//         }
//     }
//
//     LOG(INFO) << "knn ran in: " << timer.getDuration();
//
//     return points;
// }
