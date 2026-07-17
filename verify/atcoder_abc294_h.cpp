#include <bits/stdc++.h>

#include "graph/coloring.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    long long k;
    cin >> n >> m >> k;
    undirected_graph graph(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph.add_edge(--u, --v);
    }
    cout << count_graph_colorings<998244353>(graph, k) << '\n';
}
