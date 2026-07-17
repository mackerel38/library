#include <algorithm>
#include <cassert>
#include <optional>
#include <random>
#include <vector>

#include "graph/generalmatching.hpp"

int main() {
    std::mt19937 random(412);
    for (int vertices = 0; vertices <= 12; vertices += 2) {
        for (int trial = 0; trial < 120; ++trial) {
            std::vector<poe::binarymatchingedge> edges;
            std::vector<std::vector<int>> weight(vertices, std::vector<int>(vertices, -1));
            for (int first = 0; first < vertices; ++first) {
                for (int second = first + 1; second < vertices; ++second) {
                    if (random() % 3 == 0) continue;
                    const int value = random() & 1;
                    edges.push_back({first, second, value});
                    weight[first][second] = weight[second][first] = value;
                }
            }
            const int infinity = 100;
            std::vector<int> dp(1 << vertices, infinity);
            dp[0] = 0;
            for (int mask = 1; mask < (1 << vertices); ++mask) {
                const int first = std::countr_zero(static_cast<unsigned>(mask));
                for (int second = first + 1; second < vertices; ++second) {
                    if (!(mask >> second & 1) || weight[first][second] == -1) continue;
                    dp[mask] = std::min(
                        dp[mask],
                        dp[mask ^ (1 << first) ^ (1 << second)] + weight[first][second]);
                }
            }
            const auto actual =
                poe::minimum_binary_perfect_matching_weight(vertices, edges);
            const int expected = dp.back();
            assert(actual.has_value() == (expected < infinity));
            if (actual) assert(*actual == expected);
        }
    }
}
