#include <bits/stdc++.h>
#include "graph/thresholdcomponents.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(235);
    using mint = modint998244353;
    for (int trial = 0; trial < 3000; ++trial) {
        const int n = 1 + random() % 8;
        const int maximum = random() % 5;
        undirected_graph<int> graph(n);
        for (int edge = 0; edge < 12; ++edge) {
            if (random() % 2) graph.add_edge(random() % n, random() % n, 1 + random() % 4);
        }
        vector<unsigned> operations;
        for (int vertex = 0; vertex < n; ++vertex) operations.push_back(1U << vertex);
        for (int threshold = 1; threshold <= 4; ++threshold) {
            dsu components(n);
            for (const auto& edge : graph.edges()) {
                if (edge.cost <= threshold) components.merge(edge.from, edge.to);
            }
            for (const auto& group : components.groups()) {
                unsigned mask = 0;
                for (const int vertex : group) mask |= 1U << vertex;
                operations.push_back(mask);
            }
        }
        ranges::sort(operations);
        operations.erase(unique(operations.begin(), operations.end()), operations.end());
        vector<int> distance(1 << n, n + 1);
        distance[0] = 0;
        for (int mask = 0; mask < (1 << n); ++mask) {
            for (const unsigned operation : operations) {
                distance[mask | operation] = min(distance[mask | operation], distance[mask] + 1);
            }
        }
        vector<mint> expected(maximum + 1);
        for (const int value : distance) {
            if (value <= maximum) ++expected[value];
        }
        assert(threshold_component_union_counts<mint>(graph, maximum) == expected);
    }
}
