#include <bits/stdc++.h>
#include "flow/vertexcut.hpp"

using namespace std;
using namespace poe;

bool disconnected(const undirected_graph<>& graph, int removed, int source, int sink) {
    vector<char> seen(graph.size());
    queue<int> queue;
    seen[source] = true;
    queue.push(source);
    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop();
        for (const auto& edge : graph[vertex]) {
            if (removed >> edge.to & 1 || seen[edge.to]) continue;
            seen[edge.to] = true;
            queue.push(edge.to);
        }
    }
    return !seen[sink];
}

int main() {
    mt19937 random(239);
    for (int trial = 0; trial < 3000; ++trial) {
        const int n = 2 + random() % 7;
        undirected_graph<> graph(n);
        for (int left = 0; left < n; ++left) {
            for (int right = left + 1; right < n; ++right) {
                if (random() % 3 == 0) graph.add_edge(left, right);
            }
        }
        vector<optional<long long>> costs(n);
        for (int vertex = 1; vertex + 1 < n; ++vertex) {
            if (random() % 4) costs[vertex] = 1 + random() % 10;
        }
        long long expected = numeric_limits<long long>::max();
        for (int mask = 0; mask < (1 << n); ++mask) {
            bool valid = !(mask & 1) && !(mask >> (n - 1) & 1);
            long long value = 0;
            for (int vertex = 0; vertex < n && valid; ++vertex) {
                if (!(mask >> vertex & 1)) continue;
                if (!costs[vertex]) valid = false;
                else value += *costs[vertex];
            }
            if (valid && disconnected(graph, mask, 0, n - 1)) {
                expected = min(expected, value);
            }
        }
        const auto result = minimum_vertex_cut(graph, costs, 0, n - 1);
        assert(result.possible == (expected != numeric_limits<long long>::max()));
        if (!result.possible) continue;
        assert(result.cost == expected);
        int mask = 0;
        long long value = 0;
        for (const int vertex : result.vertices) {
            assert(costs[vertex]);
            mask |= 1 << vertex;
            value += *costs[vertex];
        }
        assert(value == result.cost);
        assert(disconnected(graph, mask, 0, n - 1));
    }
}
