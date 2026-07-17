#include <cassert>
#include <vector>
#include "geometry/closestpair.hpp"

using namespace std;
using namespace poe;

int main() {
    vector<point<long long>> points = {{0, 0}, {10, 10}, {2, 1}, {-5, 3}};
    auto result = closest_pair(points);
    assert(result.exists());
    assert(result.first == 0 && result.second == 2);
    assert(result.squared_distance == 5);

    assert(!closest_pair(vector<point<int>>{{1, 2}}).exists());
}
