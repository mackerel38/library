#include <bits/stdc++.h>
#include "geometry/distancepairs.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(234);
    for (int trial = 0; trial < 5000; ++trial) {
        const int n = random() % 30;
        const long long radius = 1 + random() % 20;
        vector<point<long long>> points(n);
        for (auto& point : points) {
            point.x = static_cast<int>(random() % 101) - 50;
            point.y = static_cast<int>(random() % 101) - 50;
        }
        vector<pair<int, int>> expected;
        for (int left = 0; left < n; ++left) {
            for (int right = left + 1; right < n; ++right) {
                const __int128 dx = points[left].x - points[right].x;
                const __int128 dy = points[left].y - points[right].y;
                if (dx * dx + dy * dy <= static_cast<__int128>(radius) * radius) {
                    expected.emplace_back(left, right);
                }
            }
        }
        assert(pairs_within_distance(points, radius) == expected);
    }
}
