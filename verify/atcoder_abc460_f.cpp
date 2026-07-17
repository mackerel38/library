#define PROBLEM "https://atcoder.jp/contests/abc460/tasks/abc460_f"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/vertexsetdiameter.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    undirected_graph<> graph(n);
    for (int i = 0; i + 1 < n; ++i) {
        int u, v;
        cin >> u >> v;
        graph.add_edge(--u, --v);
    }
    vertexsetdiameter diameter(graph);
    int query_count;
    cin >> query_count;
    while (query_count--) {
        int vertex;
        cin >> vertex;
        diameter.flip(--vertex);
        cout << diameter.get().distance << '\n';
    }
}
