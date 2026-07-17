#include <cassert>
#include <random>
#include <vector>

#include "tree/treemetric.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 500; ++trial) {
        const int n = 1 + random() % 10;
        std::vector<std::vector<std::pair<int, long long>>> graph(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            const int parent = random() % vertex;
            const long long cost = 1 + random() % 20;
            graph[vertex].push_back({parent, cost});
            graph[parent].push_back({vertex, cost});
        }
        std::vector distance(n, std::vector<long long>(n));
        for (int source = 0; source < n; ++source) {
            std::vector<std::tuple<int, int, long long>> stack{{source, -1, 0}};
            while (!stack.empty()) {
                auto [vertex, parent, length] = stack.back();
                stack.pop_back();
                distance[source][vertex] = length;
                for (auto [next, cost] : graph[vertex]) {
                    if (next != parent) stack.emplace_back(next, vertex, length + cost);
                }
            }
        }
        auto restored = poe::restore_tree_metric(distance);
        assert(restored && restored->edge_count() == n - 1);
        if (n >= 2) {
            ++distance[0][1];
            assert(!poe::is_tree_metric(distance));
        }
    }
}
