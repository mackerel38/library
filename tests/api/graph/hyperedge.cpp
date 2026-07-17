#include <cassert>
#include <vector>
#include "graph/graph.hpp"
#include "graph/hyperedge.hpp"
int main() {
    poe::undirected_graph<> graph(6);
    graph.add_edge(0, 1);
    graph.add_edge(4, 5);
    const auto distance = poe::bfs_with_hyperedges(graph, {{1, 3, 4}}, 0);
    assert(distance == std::vector<int>({0, 1, -1, 2, 2, 3}));
}
