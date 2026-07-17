#include "tree/virtualtree.hpp"

int main() {
    std::mt19937 random(712983);
    for (int trial = 0; trial < 3000; ++trial) {
        const int n = 1 + random() % 50;
        poe::undirected_graph<> graph(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            graph.add_edge(vertex, random() % vertex);
        }
        poe::virtualtree builder(graph);
        std::vector<int> selected;
        for (int vertex = 0; vertex < n; ++vertex) {
            if (random() % 3 == 0) selected.push_back(vertex);
        }
        if (selected.empty()) selected.push_back(random() % n);
        const auto tree = builder.build(selected);
        long long actual = 0;
        std::vector<int> count(tree.size());
        for (int node = tree.size() - 1; node >= 0; --node) {
            count[node] += tree.selected[node];
            if (tree.parent[node] != -1) {
                actual += 1LL * tree.edge_length[node] * count[node] *
                          (static_cast<int>(selected.size()) - count[node]);
                count[tree.parent[node]] += count[node];
            }
        }
        long long expected = 0;
        for (int left = 0; left < static_cast<int>(selected.size()); ++left) {
            for (int right = left + 1; right < static_cast<int>(selected.size()); ++right) {
                expected += builder.edge_distance(selected[left], selected[right]);
            }
        }
        assert(actual == expected);
        assert(tree.size() <= 2 * static_cast<int>(selected.size()) - 1);
    }
}
