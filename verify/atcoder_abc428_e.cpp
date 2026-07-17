#define PROBLEM "https://atcoder.jp/contests/abc428/tasks/abc428_e"
#include <bits/stdc++.h>
#include "tree/diameter.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    undirected_graph graph(n);
    for (int edge = 0; edge + 1 < n; ++edge) {
        int u, v;
        cin >> u >> v;
        graph.add_edge(u - 1, v - 1);
    }
    for (const auto answer : tree_farthest_vertices(graph)) {
        cout << answer.vertex + 1 << '\n';
    }
}
