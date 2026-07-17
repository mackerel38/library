#include "graph/cycle.hpp"
#include "graph/graph.hpp"

int main() {
    poe::undirected_graph<long long> graph(4);
    graph.add_edge(0, 1, 2);
    graph.add_edge(1, 2, 3);
    graph.add_edge(2, 0, 4);
    graph.add_edge(0, 3, 1);
    graph.add_edge(3, 2, 1);
    const auto cycle = poe::minimum_cycle_through_vertex(graph, 0);
    assert(cycle.found() && cycle.cost == 6);
    assert(cycle.vertices[0] == 0);
    assert(!poe::minimum_cycle_through_vertex(graph, 1, 0).found());
}
