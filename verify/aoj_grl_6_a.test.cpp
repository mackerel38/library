#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A"
#include <bits/stdc++.h>
#include "flow/dinic.hpp"
#include "flow/flowgraph.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices, edges;
    cin >> vertices >> edges;
    flowgraph<long long> graph(vertices);
    while (edges--) {
        int from, to;
        long long capacity;
        cin >> from >> to >> capacity;
        graph.add_edge(from, to, capacity);
    }
    cout << dinic(graph, 0, vertices - 1) << '\n';
}
