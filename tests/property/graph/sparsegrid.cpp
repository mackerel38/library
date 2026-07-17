#include <cassert>
#include <queue>
#include <vector>

#include "graph/grid.hpp"

int main() {
    for (int height = 1; height <= 4; ++height) {
        for (int width = 1; width <= 4; ++width) {
            std::vector<poe::gridpoint> cells;
            for (int row = 0; row < height; ++row) {
                for (int column = 0; column < width; ++column) {
                    if ((row == 0 && column == 0) ||
                        (row + 1 == height && column + 1 == width)) continue;
                    cells.push_back({row, column});
                }
            }
            for (int mask = 0; mask < (1 << cells.size()); ++mask) {
                std::vector<poe::gridpoint> blocked;
                std::vector forbidden(height, std::vector<bool>(width));
                for (int index = 0; index < static_cast<int>(cells.size()); ++index) {
                    if (!(mask >> index & 1)) continue;
                    blocked.push_back(cells[index]);
                    forbidden[cells[index].row][cells[index].column] = true;
                }
                std::queue<poe::gridpoint> queue;
                std::vector seen(height, std::vector<bool>(width));
                queue.push({0, 0});
                seen[0][0] = true;
                while (!queue.empty()) {
                    const auto current = queue.front();
                    queue.pop();
                    for (const auto direction : poe::grid4) {
                        const int row = current.row + direction.row;
                        const int column = current.column + direction.column;
                        if (!poe::inside_grid(row, column, height, width) ||
                            forbidden[row][column] || seen[row][column]) continue;
                        seen[row][column] = true;
                        queue.push({row, column});
                    }
                }
                assert(poe::sparse_grid_path_exists(height, width, blocked) ==
                       seen[height - 1][width - 1]);
            }
        }
    }
}
