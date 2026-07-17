#include "tree/difference.hpp"
#include "tree/rootedtree.hpp"

int main() {
    std::mt19937 random(812739);
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = 1 + random() % 50;
        poe::undirected_graph<> graph(n);
        for (int vertex = 1; vertex < n; ++vertex) graph.add_edge(vertex, random() % vertex);
        poe::vertexpathdifference<decltype(graph), long long> vertex_diff(graph);
        poe::edgepathdifference<decltype(graph), long long> edge_diff(graph);
        poe::subtreedifference<decltype(graph), long long> subtree_diff(graph);
        poe::rootedtree rooted(graph);
        std::vector<long long> expected_vertex(n), expected_edge(n - 1), expected_subtree(n);
        for (int operation = 0; operation < 200; ++operation) {
            const int from = random() % n;
            const int to = random() % n;
            const long long value = static_cast<int>(random() % 21) - 10;
            vertex_diff.add(from, to, value);
            edge_diff.add(from, to, value);
            std::vector<int> previous(n, -1), previous_edge(n, -1);
            std::queue<int> queue;
            previous[from] = from;
            queue.push(from);
            while (!queue.empty()) {
                const int current = queue.front();
                queue.pop();
                for (const auto& edge : graph[current]) {
                    if (previous[edge.to] != -1) continue;
                    previous[edge.to] = current;
                    previous_edge[edge.to] = edge.id;
                    queue.push(edge.to);
                }
            }
            for (int current = to;; current = previous[current]) {
                expected_vertex[current] += value;
                if (current == from) break;
                expected_edge[previous_edge[current]] += value;
            }

            const int root = random() % n;
            const long long subtree_value = static_cast<int>(random() % 21) - 10;
            if (random() % 2) {
                subtree_diff.add(root, subtree_value);
                for (int v = 0; v < n; ++v) {
                    if (rooted.is_ancestor(root, v)) expected_subtree[v] += subtree_value;
                }
            } else {
                subtree_diff.add_complement(root, subtree_value);
                for (int v = 0; v < n; ++v) {
                    if (!rooted.is_ancestor(root, v)) expected_subtree[v] += subtree_value;
                }
            }
        }
        vertex_diff.build();
        edge_diff.build();
        subtree_diff.build();
        assert(vertex_diff.values() == expected_vertex);
        assert(edge_diff.values() == expected_edge);
        assert(subtree_diff.values() == expected_subtree);
    }
}
