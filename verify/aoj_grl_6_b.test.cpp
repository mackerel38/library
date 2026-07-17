#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B"
#include <bits/stdc++.h>
#include "flow/mincostflow.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices, edges;
    long long required;
    cin >> vertices >> edges >> required;
    mincostflow<long long, long long> graph(vertices);
    while (edges--) {
        int from, to;
        long long capacity, cost;
        cin >> from >> to >> capacity >> cost;
        graph.add_edge(from, to, capacity, cost);
    }
    const auto [flow, cost] = graph.flow(0, vertices - 1, required);
    cout << (flow == required ? cost : -1) << '\n';
}
