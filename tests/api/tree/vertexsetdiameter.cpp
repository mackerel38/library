#include <cassert>
#include <vector>

#include "graph/graph.hpp"
#include "tree/vertexsetdiameter.hpp"

int main() {
    poe::undirected_graph<> graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(3, 4);
    poe::vertexsetdiameter diameter(graph);
    assert(diameter.get().distance == 3);
    diameter.set(2, false);
    diameter.flip(0);
    assert(!diameter.active(0) && diameter.get().distance == 2);
    poe::vertexsetdiameter empty(graph, std::vector<bool>(5));
    assert(empty.get().empty());
    empty.set(4, true);
    assert(empty.get().left == 4 && empty.get().right == 4);
}
