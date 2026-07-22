#include <bits/stdc++.h>
#include "graph/bottleneck.hpp"
#include "graph/graph.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    undirected_graph<long long> graph(n);
    graph.read(m, 1);
    int q;
    cin >> q;
    vector<terminalhopquery<long long>> queries(q);
    for (auto& [from, to, limit] : queries) {
        cin >> from >> to >> limit;
        --from;
        --to;
    }
    vector<int> terminals(k);
    iota(terminals.begin(), terminals.end(), 0);
    for (const bool reachable : terminal_hop_connectivity(graph, terminals, queries)) {
        cout << (reachable ? "Yes" : "No") << '\n';
    }
}
