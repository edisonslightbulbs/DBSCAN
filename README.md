#### Usage of this tiny dbscan lib based on [nanoflann](https://github.com/jlblancoc/nanoflann)

Massive thanks to [Jose](https://github.com/jlblancoc) for sharing this adaptation of [flann](https://github.com/mariusmuja/flann) 👏🍻🍻

*   the interface

```cpp

#include <vector>
#include "point.h"

namespace dbscan {

/** cluster
 *   Does density-based spatial clustering.
 *
 * @param points
 *   Un-clustered points.
 *
 * @param epsilon
 *   Epsilon hyper-parameter.
 *
 * @param minPoints
 *   Number of points in epsilon neighourhood
 *
 * @return
 *   Clustered indexes.
 */
std::vector<std::vector<unsigned long>> cluster(
std::vector<Point>& points, const float& epsilon, const int& minPoints);
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
std::vector<std::vector<unsigned long>> iClusters = dbscan::cluster(points, epsilon, minPoints);

return 0;
}
```

*   You can find an example Point Class [here](https://github.com/edisonslightbulbs/point).

*   The `readPoints` function can be implemented flexibly and therefore, left out.
