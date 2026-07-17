#include <cassert>
#include "graph/activation.hpp"

int main() {
    for (int height = 1; height <= 4; ++height) {
        for (int width = 1; width <= 4; ++width) {
            const int size = height * width;
            if (size > 16) continue;
            for (int initial_mask = 0; initial_mask < (1 << size); ++initial_mask) {
                std::vector active(height, std::vector<bool>(width));
                std::vector expected(height, std::vector<int>(width, -1));
                for (int row = 0; row < height; ++row) {
                    for (int column = 0; column < width; ++column) {
                        if (initial_mask >> (row * width + column) & 1) {
                            active[row][column] = true;
                            expected[row][column] = 0;
                        }
                    }
                }
                for (int round = 1;; ++round) {
                    std::vector<poe::gridpoint> next;
                    for (int row = 0; row < height; ++row) {
                        for (int column = 0; column < width; ++column) {
                            if (active[row][column]) continue;
                            int count = 0;
                            for (const auto direction : poe::grid4) {
                                const int next_row = row + direction.row;
                                const int next_column = column + direction.column;
                                if (poe::inside_grid(next_row, next_column, height, width)
                                    && active[next_row][next_column]) {
                                    ++count;
                                }
                            }
                            if (count == 1) next.push_back({row, column});
                        }
                    }
                    if (next.empty()) break;
                    for (const auto point : next) {
                        active[point.row][point.column] = true;
                        expected[point.row][point.column] = round;
                    }
                }

                const auto actual = poe::grid_synchronous_activation(
                    height, width,
                    [&](int row, int column) {
                        return initial_mask >> (row * width + column) & 1;
                    },
                    [](int, int, int active_neighbors) {
                        return active_neighbors == 1;
                    });
                assert(actual.round == expected);
            }
        }
    }
}
