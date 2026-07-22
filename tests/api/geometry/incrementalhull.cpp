#include <bits/stdc++.h>
#include "geometry/incrementalhull.hpp"

using namespace std;
using namespace poe;

int main() {
    incrementalconvexhull<long long> hull;
    assert(hull.empty());
    hull.add({1, 0});
    assert(hull.max_dot(-1, -1) == -1);
    hull.add({0, 1});
    assert(hull.max_dot(2, 0) == 2);
    hull.add({-1, 0});
    assert(hull.max_dot(1, 1) == 1);
    hull.add({0, -1});
    assert(hull.max_dot(1, -2) == 2);
    assert(hull.size() == 4);
}
