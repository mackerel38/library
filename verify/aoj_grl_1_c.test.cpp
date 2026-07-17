#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_C"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/warshallfloyd.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices, edges;
    cin >> vertices >> edges;
    directed_graph<long long> graph(vertices);
    while (edges--) {
        int from, to;
        long long cost;
        cin >> from >> to >> cost;
        graph.add_edge(from, to, cost);
    }
    const auto result = warshall_floyd(graph);
    if (result.has_negative_cycle()) {
        cout << "NEGATIVE CYCLE\n";
        return 0;
    }
    for (int from = 0; from < vertices; ++from) {
        for (int to = 0; to < vertices; ++to) {
            if (to) cout << ' ';
            if (!result.reachable(from, to)) cout << "INF";
            else cout << result(from, to);
        }
        cout << '\n';
    }
}
