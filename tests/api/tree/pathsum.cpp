#include <cassert>

#include "tree/pathsum.hpp"

int main() {
    poe::undirected_graph<long long> graph(5);
    graph.add_edge(0, 1, 3);
    graph.add_edge(0, 2, 6);
    graph.add_edge(0, 3, 9);
    graph.add_edge(3, 4, 10);
    poe::edgepathsum data(graph);
    assert(data.sum(1, 2) == 9);
    assert(data.sum(0, 4) == 19);
    data.set(2, 1);
    assert(data[2] == 1);
    assert(data.sum(0, 4) == 11);
    data.add(3, 4);
    assert(data.sum(3, 4) == 14);
}
