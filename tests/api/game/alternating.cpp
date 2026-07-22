#include "game/alternating.hpp"

using namespace poe;

int main() {
    directed_graph<long long> graph(4);
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 0, 1);
    graph.add_edge(1, 3, 1);
    const auto result = alternating_path_game(graph);
    assert(result.finite(0, gameplayer::minimize));
    assert(result.value(0, gameplayer::minimize) == 5);
    assert(result.value(0, gameplayer::maximize) == 2);
    assert(result.value(3, gameplayer::minimize) == 0);

    directed_graph<int> cycle(2);
    cycle.add_edge(0, 1, 1);
    cycle.add_edge(1, 0, 1);
    const auto infinite = alternating_path_game(cycle);
    assert(!infinite.finite(0, gameplayer::minimize));
    assert(!infinite.finite(0, gameplayer::maximize));
}
