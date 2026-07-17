#include <bits/stdc++.h>

#include "tree/pathsum.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    undirected_graph<long long> graph(n);
    for (int i = 0; i + 1 < n; ++i) {
        int u, v;
        long long weight;
        cin >> u >> v >> weight;
        graph.add_edge(--u, --v, weight);
    }
    edgepathsum distance(graph);
    int q;
    cin >> q;
    while (q--) {
        int type, left, right;
        cin >> type >> left >> right;
        if (type == 1) distance.set(left - 1, right);
        else cout << distance.sum(left - 1, right - 1) << '\n';
    }
}
