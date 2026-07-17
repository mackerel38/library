#include "tree/virtualtree.hpp"

int main() {
    poe::undirected_graph<> graph(7);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);
    graph.add_edge(2, 5);
    graph.add_edge(2, 6);
    poe::virtualtree builder(graph);
    const auto tree = builder.build({3, 4, 5, 5});
    assert(tree.size() == 5);
    assert(tree[0] == 0);
    assert(tree.index(3) != -1 && tree.index(6) == -1);
    assert(tree.selected[tree.index(3)]);
    assert(!tree.selected[tree.index(0)]);
    for (int node = 1; node < tree.size(); ++node) {
        assert(tree.parent[node] < node);
        assert(tree.edge_length[node] >= 1);
    }
    assert(builder.edge_distance(3, 5) == 4);
    assert(builder.build({}).root() == -1);
}
