#include <memory>
#include <vector>

#include "dbscan.h"
#include "kdtree.h"
#include "point.h"

/** color definitions for visualizing clusters */
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

void initColors()
{
    clusterColors.emplace_back(darkbrown);
    clusterColors.emplace_back(deepgreen);
    clusterColors.emplace_back(deepblue);
    clusterColors.emplace_back(yellow);
    clusterColors.emplace_back(black);
    clusterColors.emplace_back(goldenbrown);
    clusterColors.emplace_back(khaki);
    clusterColors.emplace_back(orange);
    clusterColors.emplace_back(darkgreen);
    clusterColors.emplace_back(brown);
    clusterColors.emplace_back(red);
    clusterColors.emplace_back(lightgreen);
    clusterColors.emplace_back(green);
    clusterColors.emplace_back(depbrown);
    clusterColors.emplace_back(gold);
    clusterColors.emplace_back(brown);
    clusterColors.emplace_back(yellow);
    clusterColors.emplace_back(skyblue);
    clusterColors.emplace_back(oceanblue);
    clusterColors.emplace_back(blue);
    clusterColors.emplace_back(black);
    clusterColors.emplace_back(goldenbrown);
    clusterColors.emplace_back(khaki);
    clusterColors.emplace_back(gold);
    clusterColors.emplace_back(lightgrey);
    clusterColors.emplace_back(skyblue);
    clusterColors.emplace_back(oceanblue);
    clusterColors.emplace_back(red);
    clusterColors.emplace_back(blue);
    clusterColors.emplace_back(brown);
    clusterColors.emplace_back(yellow);
    clusterColors.emplace_back(skyblue);
    clusterColors.emplace_back(oceanblue);
}

std::vector<std::vector<Point>> dbscan::cluster(
    std::vector<Point>& points, const float& E, const int& N)
{
    auto sptr_points = std::make_shared<std::vector<Point>>(points);

    /** kdtree::cluster returns clustered indexes */
    std::vector<std::vector<unsigned long>> clusters
        = kdtree::cluster(sptr_points, E, N);

    /** sort clusters using descending size */
    std::sort(clusters.begin(), clusters.end(),
        [](const std::vector<unsigned long>& a,
            const std::vector<unsigned long>& b) {
            return a.size() > b.size();
        });

    /** create objects (labeled/classified clusters) container */
    std::vector<std::vector<Point>> densityClusters;
    std::vector<Point> accumulatedClusters;

    initColors();
    int colorIndex = 0;
    int clusterLabel = 0;
    int maxClusters = 20;
    for (const auto& cluster : clusters) {
        if (cluster.size() < 20) {
            continue;
        }

        /** after 25 clusters are found, absorb the rest into largest cluster */
        if (clusterLabel >= maxClusters) {
            for (const auto& index : cluster) {
                (*sptr_points)[index].m_clusterColor = clusterColors[0];
                (*sptr_points)[index].m_cluster = 0;
                densityClusters[0].emplace_back((*sptr_points)[index]);
            }
            continue;
        }

        std::vector<Point> densityCluster;
        for (const auto& index : cluster) {
            (*sptr_points)[index].m_clusterColor = clusterColors[colorIndex];
            (*sptr_points)[index].m_cluster = clusterLabel;
            densityCluster.emplace_back((*sptr_points)[index]);
            accumulatedClusters.emplace_back((*sptr_points)[index]);
        }
        densityClusters.emplace_back(densityCluster);
        colorIndex += 1;
        clusterLabel += 1;
    }
    densityClusters.emplace_back(accumulatedClusters);
    return densityClusters;
}
