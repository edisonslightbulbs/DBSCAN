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

void collectColors()
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

std::vector<std::vector<Point>> dbscan::cluster(
    std::vector<Point>& points, const float& E, const int& N)
{
    auto sptr_points = std::make_shared<std::vector<Point>>(points);

    /** kdtree::cluster returns clusters of indexes.
     *  use clustered indexes to labeled/classified and colorize cluster points
     */
    std::vector<std::vector<unsigned long>> clusters
        = kdtree::cluster(sptr_points, E, N);

    /** sort clusters in descending order of cluster size */
    std::sort(clusters.begin(), clusters.end(),
        [](const std::vector<unsigned long>& a,
            const std::vector<unsigned long>& b) {
            return a.size() > b.size();
        });

    /** create and pre-allocate objects (labeled/classified clusters) container
     */
    std::vector<std::vector<Point>> objects(100);

    /** initialize cluster coloring */
    collectColors();
    int clusterColor = 0;
    int maxColors = clusterColors.size();

    int clusterLabel = 0;
    for (const auto& cluster : clusters) {
        std::vector<Point> object;

        /** ignore clusters with less than 100 points */
        if (cluster.size() < 100) {
            continue;
        }

        /** label clusters */
        for (const auto& index : cluster) {
            (*sptr_points)[index].m_clusterColor = clusterColors[clusterColor];
            (*sptr_points)[index].m_cluster = clusterLabel;
            object.emplace_back((*sptr_points)[index]);
        }

        /** collect labeled objects */
        objects.emplace_back(object);
        clusterLabel += 1;
        clusterColor += 1;

        /** confine cluster coloring to existing color set */
        if (clusterColor == maxColors) {
            clusterColor = 0;
        }
    }
    return objects;
}
