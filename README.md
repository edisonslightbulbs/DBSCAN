#### Usage of this tiny dbscan lib based on [nanoflann](https://github.com/jlblancoc/nanoflann)

Massive thanks to [Jose](https://github.com/jlblancoc) for sharing this adaptation of [flann](https://github.com/mariusmuja/flann) 👏🍻🍻

*   the interface

```cpp

#include <vector>
#include "point.h"

namespace dbscan {

/** dbscan::cluster
 *   Carries out view-based spatial clustering.
 *
 * @param points
 *   Unlabeled/unclustered set of points.
 *
 * @param E
 *   Epsilon hyper-parameter.
 *
 * @param N
 *   Number of points within an epsilon neighourhood
 *
 * @return
 *   Labeled/clustered set of Points.
 */
std::vector<std::vector<Point>> cluster(std::vector<Point>& points, const float& E, const int& N);
}

```

here's an example of how to use the interface

```cpp

#include <vector>
#include "dbscan.h" //<-- include dbscan library

int main(int argc, char* argv[]){

    std::vector<Point> points = readPoints();

    int minPoints = 4;
    float epsilon = 3.320;
    std::vector<std::vector<Point>> clusters = dbscan::cluster(points, epsilon, minPoints);

    writePoints(clusters.front());

    return 0;
}
```

*   You can find an example Point Class [here](https://github.com/edisonslightbulbs/point).

*   The `readPoints` and `writePoints` functions are adaptable implementations and therefore left out.
