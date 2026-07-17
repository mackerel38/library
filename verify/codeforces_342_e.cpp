#include <bits/stdc++.h>
#include "recipe/nearestactive.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    undirected_graph<> graph(n);
    for (int edge = 0; edge + 1 < n; ++edge) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(left - 1, right - 1);
    }
    nearestactive active(graph);
    active.add(0);
    while (q--) {
        int type, vertex;
        cin >> type >> vertex;
        --vertex;
        if (type == 1) active.add(vertex);
        else cout << *active.distance(vertex) << '\n';
    }
}
