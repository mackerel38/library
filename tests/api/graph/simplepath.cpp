#include <cassert>
#include "graph/simplepath.hpp"

int main() {
    poe::undirected_graph<> graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(1, 3);
    graph.add_edge(0, 2);
    graph.add_edge(2, 3);
    graph.add_edge(1, 2);
    assert((poe::simple_path_length_counts(graph, 0, 3) ==
            std::vector<long long>{0, 0, 2, 2}));
}
