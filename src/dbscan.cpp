#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "dbscan.h"
#include "point.h"

const int FAIL = -3;
const int PASS = 0;

/** see@ https://colorbrewer2.org/#type=diverging&scheme=RdYlBu&n=9 */
#define red " 215 48 39"
#define orange " 244 109 67"
#define gold " 253 173 97"
#define brown " 254 224 144"
#define yellow " 255 255 191"
#define skyblue " 224 243 248"
#define oceanblue " 171 217 233"
#define blue " 116 173 209"
#define deepblue " 69 117 180"

/** see@ https://colorbrewer2.org/#type=diverging&scheme=BrBG&n=9 */
#define depbrown " 140 81 10"
#define darkbrown " 191 129 45"
#define goldenbrown " 223 194 125"
#define khaki " 223 232 195"
#define lightgrey " 245 245 245"
#define lightgreen " 199 234 229"
#define green " 128 205 193"
#define deepgreen " 53 151 143"
#define darkgreen " 1 102 94"
#define black " 0 0 0"

std::vector<std::string> clusterColors;

void initializeClusterColors()
{
    clusterColors.emplace_back(red);
    clusterColors.emplace_back(orange);
    clusterColors.emplace_back(gold);
    clusterColors.emplace_back(brown);
    clusterColors.emplace_back(yellow);
    clusterColors.emplace_back(skyblue);
    clusterColors.emplace_back(oceanblue);
    clusterColors.emplace_back(blue);
    clusterColors.emplace_back(deepblue);
    clusterColors.emplace_back(black);
    clusterColors.emplace_back(depbrown);
    clusterColors.emplace_back(darkbrown);
    clusterColors.emplace_back(goldenbrown);
    clusterColors.emplace_back(khaki);
    clusterColors.emplace_back(lightgrey);
    clusterColors.emplace_back(lightgreen);
    clusterColors.emplace_back(green);
    clusterColors.emplace_back(deepgreen);
    clusterColors.emplace_back(darkgreen);
}

std::vector<int> queryRange(std::shared_ptr<std::vector<Point>>& sptr_points,
    const Point& core, const float& epsilon)
{
    int index = 0;
    std::vector<int> neighbours;
    for (const auto& point : *sptr_points) {
        if (core.distance(point) <= epsilon) {
            neighbours.push_back(index);
        }
        index++;
    }
    return neighbours;
}

int scan(std::shared_ptr<std::vector<Point>>& sptr_points, Point point,
    int cluster, int colorIndex, const int& minPoints, const float& epsilon)
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
        sptr_points->at(neighbour).m_clusterColor = clusterColors[colorIndex];
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
                if (sptr_points->at(neighbour).unlabeled()) {
                    neighbours.push_back(neighbour);
                    n = neighbours.size();
                    sptr_points->at(neighbour).m_cluster = cluster;
                    sptr_points->at(neighbour).m_clusterColor
                        = clusterColors[colorIndex];
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
    int maxColorIndex = clusterColors.size();
    int colorIndex = 0;

    for (const auto& point : *sptr_points) {
        if (point.m_cluster == UNLABELED) {
            if (scan(
                    sptr_points, point, cluster, colorIndex, minPoints, epsilon)
                != FAIL) {
                cluster += 1;
                colorIndex += 1;
                /** reset color index after max */
                if (colorIndex == maxColorIndex) {
                    colorIndex = 0;
                }
                std::cout << cluster << std::endl;
            }
        }
    }
    return { *sptr_points, cluster };
}

std::pair<std::vector<Point>, int> dbscan::original(std::vector<Point>& points)
{
    /** initialize cluster colors */
    initializeClusterColors();

    /** set epsilon */
    float epsilon = 3.317;
    const int MIN_POINTS = 4;

    /** cluster points and track number of clusters */
    auto sptr_points = std::make_shared<std::vector<Point>>(points);
    std::pair<std::vector<Point>, int> clusters;
    clusters = cluster(sptr_points, MIN_POINTS, epsilon);
    return clusters;
}
