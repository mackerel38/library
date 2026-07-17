#define PROBLEM "https://atcoder.jp/contests/abc308/tasks/abc308_h"
#include <bits/stdc++.h>
#include "graph/cycle.hpp"
#include "graph/graph.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    undirected_graph<long long> graph(n);
    for (int edge = 0; edge < m; ++edge) {
        int left, right;
        long long cost;
        cin >> left >> right >> cost;
        graph.add_edge(left - 1, right - 1, cost);
    }

    constexpr long long infinity = numeric_limits<long long>::max() / 4;
    long long answer = infinity;
    for (int root = 0; root < n; ++root) {
        const auto cycle = minimum_cycle_through_vertex(graph, root);
        if (!cycle.found()) continue;
        const int first = cycle.vertices[1];
        const int last = cycle.vertices.back();
        for (const auto& edge : graph[root]) {
            if (edge.to != first && edge.to != last) {
                answer = min(answer, cycle.cost + edge.cost);
            }
        }
        for (const int edge_id : {cycle.edge_ids.front(), cycle.edge_ids.back()}) {
            const auto alternative =
                minimum_cycle_through_vertex(graph, root, edge_id);
            if (alternative.found()) {
                answer = min(
                    answer, alternative.cost + graph.edge_at(edge_id).cost);
            }
        }
    }
    cout << (answer == infinity ? -1 : answer) << '\n';
}

