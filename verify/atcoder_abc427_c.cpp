#define PROBLEM "https://atcoder.jp/contests/abc427/tasks/abc427_c"
#include <bits/stdc++.h>
#include "graph/maximumcut.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    undirected_graph graph(n);
    for (int edge = 0; edge < m; ++edge) {
        int u, v;
        cin >> u >> v;
        graph.add_edge(u - 1, v - 1);
    }
    cout << m - maximum_cut(graph).weight << '\n';
}
