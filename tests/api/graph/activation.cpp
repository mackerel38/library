#include <cassert>
#include "graph/activation.hpp"

int main() {
    poe::undirected_graph graph(5);
    for (int vertex = 0; vertex + 1 < 5; ++vertex) {
        graph.add_edge(vertex, vertex + 1);
    }
    const auto result = poe::synchronous_activation(
        graph, std::vector<bool>{true, false, false, false, true},
        [](int, int active_neighbors) { return active_neighbors >= 1; });
    assert((result.round == std::vector<int>{0, 1, 2, 1, 0}));
    assert(result.active_count == 5);

    const std::vector<std::string> grid{"...", ".#.", "..."};
    const auto spread = poe::grid_synchronous_activation(
        3, 3,
        [&](int row, int column) { return grid[row][column] == '#'; },
        [](int, int, int active_neighbors) { return active_neighbors == 1; });
    assert(spread.active_count == 5);
    assert(spread.round[0][0] == -1 && !spread.contains(2, 2));
}
