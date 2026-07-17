#define PROBLEM "https://atcoder.jp/contests/abc429/tasks/abc429_e"
#include <bits/stdc++.h>
#include "graph/shortestpath.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    undirected_graph graph(n);
    while (m--) {
        int u, v;
        cin >> u >> v;
        graph.add_edge(u - 1, v - 1);
    }
    string state;
    cin >> state;
    vector<int> safe;
    for (int vertex = 0; vertex < n; ++vertex) {
        if (state[vertex] == 'S') safe.push_back(vertex);
    }
    const auto nearest = nearest_sources_bfs<2>(graph, safe);
    for (int vertex = 0; vertex < n; ++vertex) {
        if (state[vertex] == 'D') {
            cout << nearest[vertex][0].distance + nearest[vertex][1].distance << '\n';
        }
    }
}
