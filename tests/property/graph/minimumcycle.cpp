#include "graph/cycle.hpp"
#include "graph/graph.hpp"

long long brute(
    const poe::undirected_graph<long long>& graph,
    int root,
    int forbidden
) {
    constexpr long long infinity = std::numeric_limits<long long>::max() / 4;
    long long answer = infinity;
    std::vector<char> used(graph.size());
    used[root] = true;
    const auto dfs = [&](this auto&& self, int vertex, long long cost, int length) -> void {
        for (const auto& edge : graph[vertex]) {
            if (edge.id == forbidden) continue;
            if (edge.to == root) {
                if (length >= 2) answer = std::min(answer, cost + edge.cost);
            } else if (!used[edge.to]) {
                used[edge.to] = true;
                self(edge.to, cost + edge.cost, length + 1);
                used[edge.to] = false;
            }
        }
    };
    dfs(root, 0, 0);
    return answer;
}

int main() {
    std::mt19937 random(20260716);
    constexpr long long infinity = std::numeric_limits<long long>::max() / 4;
    for (int n = 3; n <= 8; ++n) {
        for (int trial = 0; trial < 300; ++trial) {
            poe::undirected_graph<long long> graph(n);
            for (int left = 0; left < n; ++left) {
                for (int right = left + 1; right < n; ++right) {
                    if (random() % 3 == 0) {
                        graph.add_edge(left, right, 1 + random() % 20);
                    }
                }
            }
            for (int root = 0; root < n; ++root) {
                for (int forbidden = -1; forbidden < graph.edge_count(); ++forbidden) {
                    const long long expected = brute(graph, root, forbidden);
                    const auto actual =
                        poe::minimum_cycle_through_vertex(graph, root, forbidden);
                    assert(actual.found() == (expected != infinity));
                    if (actual.found()) assert(actual.cost == expected);
                }
            }
        }
    }
}
