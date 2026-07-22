#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/antichain.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    undirected_graph<> graph(n);
    for (int vertex = 1; vertex < n; ++vertex) {
        int parent;
        cin >> parent;
        graph.add_edge(parent - 1, vertex);
    }
    const auto count = tree_antichain_counts(graph);
    for (int size = 1; size <= n; ++size) cout << count[size] << '\n';
}
