#include <cassert>
#include <vector>

#include "graph/shortestpath.hpp"

int main() {
    poe::undirected_graph graph(5);
    for (int vertex = 0; vertex + 1 < 5; ++vertex) graph.add_edge(vertex, vertex + 1);
    const auto result = poe::nearest_sources_bfs<2>(graph, std::vector<int>{0, 4});
    assert(result.reachable(2, 1));
    assert(result[2][0].distance == 2 && result[2][1].distance == 2);
    assert(result[1][0].source == 0 && result[1][0].distance == 1);
}
