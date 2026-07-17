#include <cassert>
#include "game/dag.hpp"

int main() {
    poe::directed_graph<> graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 3);
    graph.add_edge(2, 4);
    const auto grundy = poe::dag_grundy(graph);
    assert(grundy[3] == 0 && grundy[4] == 0);
    assert(grundy[2] == 1 && grundy[0] == 0);
    assert(!poe::dag_winning_positions(graph)[0]);
}
