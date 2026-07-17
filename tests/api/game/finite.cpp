#include <cassert>
#include <vector>

#include "game/finite.hpp"

int main() {
    poe::directed_graph graph(2);
    graph.add_edge(0, 0);
    graph.add_edge(0, 1);
    graph.add_edge(1, 1);
    const std::vector<bool> terminal{false, true};
    const auto alternating = poe::finite_horizon_game(graph, terminal, 2);
    assert(alternating[0] && alternating[1]);
    const auto both_maximize = poe::finite_horizon_game(
        graph, terminal, 2, [](int) { return true; }
    );
    assert(both_maximize[0] && both_maximize[1]);
}
