#include <bits/stdc++.h>
#include "tree/pathfold.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260722);
    for (int iteration = 0; iteration < 4000; ++iteration) {
        const int n = uniform_int_distribution<int>(1, 10)(random);
        undirected_graph graph(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            graph.add_edge(vertex, uniform_int_distribution<int>(0, vertex - 1)(random));
        }
        vector<int> values(n);
        for (int& value : values) value = uniform_int_distribution<int>(1, 30)(random);

        auto actual = tree_path_fold_statistics(
            graph, values, [](int left, int right) { return gcd(left, right); });
        map<int, treepathfoldstat> expected;
        for (int source = 0; source < n; ++source) {
            vector<int> parent(n, -1);
            parent[source] = source;
            queue<int> queue;
            queue.push(source);
            while (!queue.empty()) {
                const int vertex = queue.front();
                queue.pop();
                for (const auto& edge : graph[vertex]) {
                    if (parent[edge.to] != -1) continue;
                    parent[edge.to] = vertex;
                    queue.push(edge.to);
                }
            }
            for (int target = source + 1; target < n; ++target) {
                int fold = values[target];
                int length = 1;
                for (int vertex = target; vertex != source; ) {
                    vertex = parent[vertex];
                    fold = gcd(fold, values[vertex]);
                    ++length;
                }
                ++expected[fold].count;
                expected[fold].length_sum += length;
            }
        }
        assert(actual.size() == expected.size());
        for (const auto& [value, state] : expected) {
            assert(actual[value].count == state.count);
            assert(actual[value].length_sum == state.length_sum);
        }
    }
}
