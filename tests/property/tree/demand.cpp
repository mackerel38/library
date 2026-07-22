#include <bits/stdc++.h>
#include "tree/demand.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260722);
    for (int trial = 0; trial < 5000; ++trial) {
        const int n = uniform_int_distribution<int>(2, 12)(random);
        undirected_graph graph(n);
        vector<vector<int>> children(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            const int parent = uniform_int_distribution<int>(0, vertex - 1)(random);
            graph.add_edge(parent, vertex);
            children[parent].push_back(vertex);
        }
        vector<long long> demand(n), allowance(n);
        vector<int> values(n);
        for (int vertex = 0; vertex < n; ++vertex) {
            demand[vertex] = random() % 4;
            allowance[vertex] = random() % 4;
            values[vertex] = random() % 10;
        }
        vector<long long> expected(n);
        const auto solve = [&](auto&& self, int vertex) -> long long {
            long long sum = 0;
            for (const int child : children[vertex]) sum += self(self, child);
            return expected[vertex] = demand[vertex] + max(0LL, sum - allowance[vertex]);
        };
        solve(solve, 0);
        assert(tree_demand_residual(graph, demand, allowance) == expected);

        optional<int> brute;
        for (int threshold = 0; threshold <= 9; ++threshold) {
            vector<long long> marked(n), one(n, 1);
            for (int vertex = 1; vertex < n; ++vertex) {
                marked[vertex] = values[vertex] >= threshold;
            }
            if (tree_demand_residual(graph, marked, one)[0] > 0) brute = threshold;
        }
        assert(maximum_tree_demand_threshold(graph, values) == brute);
    }
}
