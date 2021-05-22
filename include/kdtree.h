#include <memory>
#include <nanoflann.hpp>
#include <vector>

#include "point.h"

namespace kdtree {

struct adaptor {
    std::vector<Point> points;
    explicit adaptor(std::shared_ptr<std::vector<Point>>& sptr_points)
        : points(*sptr_points)
    {
    }

    [[nodiscard]] inline size_t kdtree_get_point_count() const
    {
        return points.size();
    }

    [[nodiscard]] inline float kdtree_get_pt(
        const size_t index, const size_t dim) const
    {
        switch (dim) {
        case 0:
            return points[index].m_xyz[0];
        case 1:
            return points[index].m_xyz[1];
        default:
            return points[index].m_xyz[2];
        }
    }

    template <class BBOX> bool kdtree_get_bbox(BBOX& /*bb*/) const
    {
        return false;
    }
};

std::array<float, 3> get_query_point(
    std::shared_ptr<std::vector<Point>>& sptr_points, size_t index)
{
    return std::array<float, 3>({ (float)(*sptr_points)[index].m_xyz[0],
        (float)(*sptr_points)[index].m_xyz[1],
        (float)(*sptr_points)[index].m_xyz[2] });
}

std::vector<std::vector<unsigned long>> dbscan(
    std::shared_ptr<std::vector<Point>>& sptr_points, float eps, int min_pts)
{
    eps *= eps;
    const auto adapt = adaptor(sptr_points);
    using index_t = nanoflann::KDTreeSingleIndexAdaptor<
        nanoflann::L2_Simple_Adaptor<float, decltype(adapt)>, decltype(adapt),
        3>;

    index_t indexAdaptor(
        3, adapt, nanoflann::KDTreeSingleIndexAdaptorParams(10));

    indexAdaptor.buildIndex();

    auto visited = std::vector<bool>(sptr_points->size());
    auto clusters = std::vector<std::vector<size_t>>();
    auto matches = std::vector<std::pair<size_t, float>>();
    auto sub_matches = std::vector<std::pair<size_t, float>>();

    for (size_t i = 0; i < sptr_points->size(); i++) {
        if (visited[i])
            continue;

        indexAdaptor.radiusSearch(get_query_point(sptr_points, i).data(), eps,
            matches, nanoflann::SearchParams(32, 0.f, false));
        if (matches.size() < static_cast<size_t>(min_pts))
            continue;
        visited[i] = true;

        std::vector<size_t> cluster = { i };

        while (!matches.empty()) {
            auto nb_idx = matches.back().first;
            matches.pop_back();
            if (visited[nb_idx])
                continue;
            visited[nb_idx] = true;

            indexAdaptor.radiusSearch(
                get_query_point(sptr_points, nb_idx).data(), eps, sub_matches,
                nanoflann::SearchParams(32, 0.f, false));

            if (sub_matches.size() >= static_cast<size_t>(min_pts)) {
                std::copy(sub_matches.begin(), sub_matches.end(),
                    std::back_inserter(matches));
            }
            cluster.push_back(nb_idx);
        }
        clusters.emplace_back(std::move(cluster));
    }
    return clusters;
}

std::vector<std::vector<unsigned long>> cluster(
    std::shared_ptr<std::vector<Point>>& sptr_points, const float& E,
    const int& N)
{
    return kdtree::dbscan(sptr_points, E, N);
}
}
