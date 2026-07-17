#include "tree/difference.hpp"

int main() {
    poe::undirected_graph<> graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(3, 4);

    poe::vertexpathdifference<decltype(graph), long long> vertex(graph);
    vertex.add(2, 4, 3);
    vertex.add(0, 2, 5);
    vertex.build();
    assert(vertex.values() == std::vector<long long>({5, 8, 8, 3, 3}));

    poe::edgepathdifference<decltype(graph), int> edge(graph);
    edge.add(2, 4, 2);
    edge.build();
    assert(edge.values() == std::vector<int>({0, 2, 2, 2}));

    poe::subtreedifference<decltype(graph), int> subtree(graph);
    subtree.add(1, 4);
    subtree.add_complement(3, 2);
    subtree.build();
    assert(subtree.values() == std::vector<int>({2, 6, 6, 4, 4}));
}
