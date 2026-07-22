#include "graph/graph.hpp"
#include "tree/antichain.hpp"

using namespace poe;

int main() {
    undirected_graph<> graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(0, 3);
    const auto count = tree_antichain_counts(graph);
    assert(count.size() == 5);
    assert(count[0] == 1 && count[1] == 4 && count[2] == 2);
    assert(count[3] == 0 && count[4] == 0);

    undirected_graph<> empty;
    assert((tree_antichain_counts(empty) == std::vector<modint998244353>{1}));
}
