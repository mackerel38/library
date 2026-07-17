#include <cassert>
#include <random>
#include <vector>
#include "utility/presetsegtree.hpp"
int main() {
    std::mt19937 random(437);
    for (int n = 1; n <= 50; ++n) {
        std::vector<std::pair<long long, long long>> points(n);
        for (auto& [x, y] : points) x = int(random() % 41) - 20, y = int(random() % 41) - 20;
        poe::point_set_range_manhattan_max<long long> data(points);
        for (int q = 0; q < 200; ++q) {
            if (random() & 1) {
                int i = random() % n; long long x = int(random() % 41) - 20, y = int(random() % 41) - 20;
                points[i] = {x, y}; data.set(i, x, y);
            } else {
                int l = random() % n, r = random() % n; if (l > r) std::swap(l, r); ++r;
                long long x = int(random() % 41) - 20, y = int(random() % 41) - 20, expected = 0;
                for (int i = l; i < r; ++i) expected = std::max(expected, std::abs(x-points[i].first)+std::abs(y-points[i].second));
                assert(data.max(l, r, x, y) == expected);
            }
        }
    }
}
