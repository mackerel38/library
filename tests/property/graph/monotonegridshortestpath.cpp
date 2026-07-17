#include <array>
#include <cassert>
#include <queue>
#include <random>
#include <vector>

#include "graph/monotonegridshortestpath.hpp"

namespace {

int brute(
    const std::vector<std::array<bool, 4>>& columns,
    int from_row,
    int from_column,
    int to_row,
    int to_column
) {
    if (!columns[from_column][from_row] || !columns[to_column][to_row]) return -1;
    const int width = static_cast<int>(columns.size());
    std::vector distance(4, std::vector<int>(width, -1));
    std::queue<std::pair<int, int>> queue;
    distance[from_row][from_column] = 0;
    queue.emplace(from_row, from_column);
    while (!queue.empty()) {
        const auto [row, column] = queue.front();
        queue.pop();
        for (const auto [next_row, next_column] : std::array{
                 std::pair{row - 1, column},
                 std::pair{row + 1, column},
                 std::pair{row, column + 1}
             }) {
            if (next_row < 0 || next_row >= 4 || next_column > to_column ||
                !columns[next_column][next_row] || distance[next_row][next_column] != -1) continue;
            distance[next_row][next_column] = distance[row][column] + 1;
            queue.emplace(next_row, next_column);
        }
    }
    return distance[to_row][to_column];
}

}

int main() {
    std::mt19937 random(429);
    for (int trial = 0; trial < 30; ++trial) {
        const int width = 1 + random() % 8;
        std::vector<std::array<bool, 4>> columns(width);
        for (auto& column : columns) {
            for (auto&& cell : column) cell = random() % 3 != 0;
        }
        poe::monotonegridshortestpath<4> path(columns);
        for (int update = 0; update < 12; ++update) {
            const int row = random() % 4;
            const int column = random() % width;
            columns[column][row] = !columns[column][row];
            path.toggle(row, column);
            for (int left = 0; left < width; ++left) {
                for (int right = left; right < width; ++right) {
                    for (int from = 0; from < 4; ++from) {
                        for (int to = 0; to < 4; ++to) {
                            assert(path.distance(from, left, to, right) ==
                                   brute(columns, from, left, to, right));
                        }
                    }
                }
            }
        }
    }
}
