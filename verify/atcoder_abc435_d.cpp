#include <bits/stdc++.h>
#include "graph/monotonereachability.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    directed_graph<> graph(n);
    for (int i = 0; i < m; ++i) {
        int from, to;
        cin >> from >> to;
        graph.add_edge(--from, --to);
    }
    monotonereachability reached(graph.reversed());
    int q;
    cin >> q;
    while (q--) {
        int type, vertex;
        cin >> type >> vertex;
        --vertex;
        if (type == 1) reached.add_source(vertex);
        else cout << (reached.contains(vertex) ? "Yes" : "No") << '\n';
    }
}
