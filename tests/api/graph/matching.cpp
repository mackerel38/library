#include "graph/matching.hpp"

int main() {
    poe::bipartitematching graph(3, 3);
    graph.add_edge(0, 0);
    graph.add_edge(0, 1);
    graph.add_edge(1, 1);
    graph.add_edge(2, 1);
    graph.add_edge(2, 2);
    const auto result = graph.solve();
    assert(result.size() == 3);
    for (int left = 0; left < 3; ++left) {
        assert(result[left] != -1);
        assert(result.right[result[left]] == left);
    }
    assert(result.left_cover.size() + result.right_cover.size() == 3);
    assert(result.independent_size() == 3);
    assert(result.left_independent().size() + result.right_independent().size() == 3);
    assert(graph.left_size() == 3 && graph.right_size() == 3);
    assert(graph.edge_count() == 5 && graph[0].size() == 2);
}
