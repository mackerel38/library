#include <bits/stdc++.h>
#include "tree/centroidtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    undirected_graph<> graph(n);
    for (int edge = 0; edge + 1 < n; ++edge) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(left - 1, right - 1);
    }
    centroidtree decomposition(graph);
    for (int vertex = 0; vertex < n; ++vertex) {
        if (vertex > 0) cout << ' ';
        const int parent = decomposition.parent(vertex);
        cout << (parent == -1 ? -1 : parent + 1);
    }
    cout << '\n';
}
