#include <vector>
#include <nanoflann.hpp>

namespace kdtree {

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

#include <vector>

// And this is the "dataset to kd-tree" adaptor class:
    struct adaptor
    {
        const std::vector<std::pair<float, float>>&  points;
        explicit adaptor(const std::vector<std::pair<float, float>>&  points) : points(points) { }

        /// CRTP helper method
        //inline const Derived& derived() const { return obj; }

        // Must return the number of data points
        [[nodiscard]] inline size_t kdtree_get_point_count() const { return points.size(); }

        // Returns the dim'th component of the idx'th point in the class:
        // Since this is inlined and the "dim" argument is typically an immediate value, the
        //  "if/else's" are actually solved at compile time.
        [[nodiscard]] inline float kdtree_get_pt(const size_t idx, const size_t dim) const
        {
            return (dim == 0)? points[idx].first: points[idx].second;
        }

        // Optional bounding-box computation: return false to default to a standard bbox computation loop.
        //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
        //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
        template <class BBOX>
        bool kdtree_get_bbox(BBOX& /*bb*/) const { return false; }

    };


    auto get_query_point(const std::vector<std::pair<float, float>>& data, size_t index)
    {
        return std::array<float, 2>({data[index].first, data[index].second});
    }


    auto sort_clusters(std::vector<std::vector<size_t>>& clusters)
    {
        for(auto& cluster: clusters)
        {
            std::sort(cluster.begin(), cluster.end());
        }
    }

    auto dbscan(const std::vector<std::pair<float, float>>& data, float eps, int min_pts)
    {
        eps *= eps;
        const auto adapt = adaptor(data);
        using namespace nanoflann;
        using  my_kd_tree_t = KDTreeSingleIndexAdaptor<L2_Simple_Adaptor<float, decltype(adapt)>, decltype(adapt), 2>;

        auto index = my_kd_tree_t(2, adapt, nanoflann::KDTreeSingleIndexAdaptorParams(10));
        index.buildIndex();

        auto visited  = std::vector<bool>(data.size());
        auto clusters = std::vector<std::vector<size_t>>();
        auto matches  = std::vector<std::pair<size_t, float>>();
        auto sub_matches = std::vector<std::pair<size_t, float>>();

        for(size_t i = 0; i < data.size(); i++)
        {
            if (visited[i]) continue;

            index.radiusSearch(get_query_point(data, i).data(), eps, matches, SearchParams(32, 0.f, false));
            if (matches.size() < static_cast<size_t>(min_pts)) continue;
            visited[i] = true;

            auto cluster = std::vector({i});

            while (!matches.empty())
            {
                auto nb_idx = matches.back().first;
                matches.pop_back();
                if (visited[nb_idx]) continue;
                visited[nb_idx] = true;

                index.radiusSearch(get_query_point(data, nb_idx).data(), eps, sub_matches, SearchParams(32, 0.f, false));

                if (sub_matches.size() >= static_cast<size_t>(min_pts))
                {
                    std::copy(sub_matches.begin(), sub_matches.end(), std::back_inserter(matches));
                }
                cluster.push_back(nb_idx);
            }
            clusters.emplace_back(std::move(cluster));
        }
        sort_clusters(clusters);
        return clusters;
    }


    int cluster(
            std::shared_ptr<std::vector<Point>>& sptr_points, const int& minPoints,
            const float& epsilon){

        std::vector<std::pair<float, float>> data;

        for(const auto& point : *sptr_points){
            data.emplace_back(point.m_xyz[0], point.m_xyz[1]);
        }
        auto clusters =  kdtree::dbscan(data, epsilon, minPoints);


        /** colorize clusters */
        int colorIndex = 0;
        kdtree::initializeClusterColors();
        int maxColorIndex = kdtree::clusterColors.size();

        int numClusters = 0;
        for(const auto& cluster : clusters){
            for (const auto& index : cluster){
                (*sptr_points)[index].m_clusterColor = kdtree::clusterColors[colorIndex];
            }
            /** count clusters */
            numClusters += 1;

            /** reset color index after max */
            colorIndex += 1;
            if (colorIndex == maxColorIndex) {
                colorIndex = 0;
            }
        }
        return numClusters;
    }
}

