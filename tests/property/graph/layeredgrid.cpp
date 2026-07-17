#include <algorithm>
#include <cassert>
#include <random>
#include <set>
#include <vector>

#include "graph/grid.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int trial = 0; trial < 500; ++trial) {
        const int layers = 1 + random() % 5;
        const int height = 1 + random() % 7;
        const int width = 1 + random() % 7;
        std::vector grid(layers, std::vector(height, std::vector<int>(width)));
        for (auto& floor : grid) for (auto& row : floor) {
            for (int& cell : row) cell = random() % 3 != 0;
        }
        const auto result = poe::layered_grid_components(
            layers, height, width,
            [&](int layer, int row, int column) { return grid[layer][row][column]; });
        for (int layer = 0; layer + 1 < layers; ++layer) {
            std::set<std::pair<int, int>> expected;
            for (int row = 0; row < height; ++row) for (int column = 0; column < width; ++column) {
                const int lower = result.layer[layer].component[row][column];
                const int upper = result.layer[layer + 1].component[row][column];
                if (lower != -1 && upper != -1) expected.emplace(lower, upper);
            }
            std::set<std::pair<int, int>> actual;
            for (int lower = 0; lower < result.layer[layer].count(); ++lower) {
                assert(std::ranges::is_sorted(result.up[layer][lower]));
                for (int upper : result.up[layer][lower]) actual.emplace(lower, upper);
            }
            assert(actual == expected);
            for (const auto& [lower, upper] : expected) {
                assert(std::ranges::binary_search(result.down[layer + 1][upper], lower));
            }
        }
    }
}
