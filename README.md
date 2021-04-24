## DBSCAN using [nanoflann's kdtree](https://github.com/jlblancoc/nanoflann)

### ... fast ... very fast!

*   simple interface

    #include "point.h"
    #include <vector>

    namespace dbscan {

    /** dbscan::cluster
     *   Carries out view-based spatial clustering.
     *
     * @param points
     *   Unlabeled (un-clustered) set of points.
     *
     * @param E
     *   Epsilon hyper-parameter.
     *
     * @param N
     *   Number of points in epsilon neighourhood
     *
     * @return
     *   Labeled (clustered) set of Points.
     */
    std::vector<std::vector<Point>> cluster(std::vector<Point>& points, const float& E, const int& N);
    }

You can find the Point Class (example) used in this implementation [here](https://github.com/edisonslightbulbs/point).
