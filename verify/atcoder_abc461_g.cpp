#define PROBLEM "https://atcoder.jp/contests/abc461/tasks/abc461_g"
#include <bits/stdc++.h>
#include "graph/matching.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertex_count, edge_count;
    cin >> vertex_count >> edge_count;
    bipartitematching graph(vertex_count, vertex_count);
    for (int i = 0; i < edge_count; ++i) {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        graph.add_edge(u, v);
        graph.add_edge(v, u);
    }
    cout << 1013LL * graph.solve().independent_size() << '\n';
}
