#include <cassert>

#include "graph/maximumcut.hpp"

int main() {
    poe::undirected_graph graph(3);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 0);
    const auto result = poe::maximum_cut(graph);
    assert(result.weight == 2);
    assert(result.side.size() == 3);
    assert(result.side[0] == false);
}
