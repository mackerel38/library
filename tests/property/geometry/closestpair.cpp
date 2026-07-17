#include <cassert>
#include <random>
#include <vector>
#include "geometry/closestpair.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260715);
    for (int n = 0; n <= 100; ++n) {
        for (int repetition = 0; repetition < 20; ++repetition) {
            vector<point<long long>> points(n);
            for (auto& current : points) {
                current.x = static_cast<int>(random() % 2001) - 1000;
                current.y = static_cast<int>(random() % 2001) - 1000;
            }
            auto result = closest_pair(points);
            if (n < 2) {
                assert(!result.exists());
                continue;
            }
            __int128 expected = -1;
            for (int first = 0; first < n; ++first) {
                for (int second = first + 1; second < n; ++second) {
                    const auto squared = detail::squared_point_distance(points[first], points[second]);
                    if (expected == -1 || squared < expected) expected = squared;
                }
            }
            assert(result.exists() && result.squared_distance == expected);
            assert(detail::squared_point_distance(points[result.first], points[result.second]) == expected);
        }
    }
}
