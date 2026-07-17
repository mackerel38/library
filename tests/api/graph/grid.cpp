#include <cassert>
#include <vector>

#include "graph/grid.hpp"

int main() {
    const std::vector<std::string> cells{"#####", "#..##", "####.", "#####"};
    auto components = poe::grid_components(4, 5, [&](int row, int column) {
        return cells[row][column] == '.';
    });
    assert(components.count() == 2);
    assert(components.size == std::vector<int>({2, 1}));
    assert(!components.touches_border[0] && components.touches_border[1]);
    const int height = 3, width = 4;
    const std::vector<std::vector<int>> wall{{0, 0, 1, 0}, {1, 0, 1, 0}, {0, 0, 0, 0}};
    const auto distance = poe::gridbfs(
        height, width, std::vector<poe::gridpoint>{{0, 0}},
        [&](int row, int column) { return !wall[row][column]; }
    );
    assert(distance[0][0] == 0 && distance[2][3] == 5 && distance[0][3] == 7);
    const auto diagonal = poe::gridbfs(
        3, 3, std::vector<poe::gridpoint>{{0, 0}},
        [](int, int) { return true; }, poe::grid8
    );
    assert(diagonal[2][2] == 2);
    assert(poe::inside_grid(2, 3, 3, 4) && !poe::inside_grid(3, 0, 3, 4));
}
