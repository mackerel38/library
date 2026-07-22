#include <bits/stdc++.h>
#include "geometry/distancepairs.hpp"

using namespace std;
using namespace poe;

int main() {
    const vector<point<long long>> points = {{0, 0}, {3, 4}, {5, 0}, {0, 0}};
    assert((pairs_within_distance(points, 5LL)
            == vector<pair<int, int>>{{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}}));
}
