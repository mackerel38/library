#include <bits/stdc++.h>
#include "geometry/incrementalhull.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937_64 random(244);
    for (int trial = 0; trial < 2000; ++trial) {
        incrementalconvexhull<long long> hull;
        vector<point<long long>> points;
        const int n = random() % 80 + 1;
        for (int index = 0; index < n; ++index) {
            point<long long> value{
                static_cast<long long>(random() % 101) - 50,
                static_cast<long long>(random() % 101) - 50};
            hull.add(value);
            points.push_back(value);
            for (int query = 0; query < 10; ++query) {
                const long long a = static_cast<long long>(random() % 101) - 50;
                const long long b = static_cast<long long>(random() % 101) - 50;
                __int128_t expected = -((__int128_t)1 << 120);
                for (const auto& current : points) {
                    expected = max(expected, (__int128_t)a * current.x
                                           + (__int128_t)b * current.y);
                }
                assert(hull.max_dot(a, b) == expected);
            }
        }
    }
}
