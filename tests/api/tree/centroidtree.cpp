#include "tree/centroidtree.hpp"

int main() {
    poe::undirected_graph<long long> graph(5);
    graph.add_edge(0, 1, 2);
    graph.add_edge(1, 2, 3);
    graph.add_edge(2, 3, 5);
    graph.add_edge(3, 4, 7);
    poe::centroidtree tree(graph);
    assert(tree.size() == 5);
    assert(tree.parent(tree.root()) == -1);
    assert(tree[0] == tree.root());
    for (int vertex = 0; vertex < 5; ++vertex) {
        const auto& path = tree.path(vertex);
        assert(path.front().centroid == tree.root());
        assert(path.back().centroid == vertex && path.back().distance == 0);
        for (int index = 1; index < static_cast<int>(path.size()); ++index) {
            assert(tree.parent(path[index].centroid) == path[index - 1].centroid);
        }
    }
    const auto& endpoint = tree.path(4);
    assert(endpoint.front().distance == 12);
}
