#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <random>
#include <utility>
#include <vector>

#include "geometry/manhattan.hpp"

using namespace poe;

int main() {
    std::mt19937 random(233);
    for (int trial = 0; trial < 3000; ++trial) {
        const int size = random() % 30 + 1;
        std::vector<std::pair<long long, long long>> points(size);
        for (auto& [x, y] : points) {
            x = static_cast<int>(random() % 101) - 50;
            y = static_cast<int>(random() % 101) - 50;
        }
        manhattannearest near(points);
        const long long x = static_cast<int>(random() % 121) - 60;
        const long long y = static_cast<int>(random() % 121) - 60;
        std::vector<long long> distances;
        for (const auto& [px, py] : points) distances.push_back(std::abs(px - x) + std::abs(py - y));
        std::sort(distances.begin(), distances.end());
        for (int k = 1; k <= size; ++k) assert(near.kth(x, y, k) == distances[k - 1]);
        const long long radius = random() % 100;
        assert(near.count(x, y, radius) ==
               static_cast<int>(std::upper_bound(distances.begin(), distances.end(), radius) - distances.begin()));
    }
}
