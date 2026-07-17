#include <cassert>
#include <functional>

#include "tree/diameter.hpp"

int main() {
    poe::undirected_graph graph(3);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    const auto largest = poe::tree_farthest_vertices(graph);
    assert(largest[1].vertex == 2 && largest[1].distance == 1);
    const auto smallest = poe::tree_farthest_vertices(graph, std::less<int>{});
    assert(smallest[1].vertex == 0 && smallest[1].distance == 1);
}
