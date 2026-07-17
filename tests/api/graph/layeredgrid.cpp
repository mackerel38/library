#include "graph/grid.hpp"

int main() {
    const std::vector<std::vector<std::string>> grid = {
        {"..#", "#.."},
        {".##", "..."},
    };
    const auto result = poe::layered_grid_components(2, 2, 3,
        [&](int layer, int row, int column) { return grid[layer][row][column] == '.'; });
    assert(result.count() == 2);
    assert(result.layer[0].count() == 1);
    assert(result.layer[1].count() == 2);
    assert(result.up[0][0].size() == 2);
    for (int upper : result.up[0][0]) {
        assert(result.down[1][upper] == std::vector<int>{0});
    }
}
