#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/incrementalreachability.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    directed_graph<> graph(n);
    vector<int> minimum_predecessor(n);
    iota(minimum_predecessor.begin(), minimum_predecessor.end(), 0);
    for (int i = 0; i < m; ++i) {
        int from, to;
        cin >> from >> to;
        --from;
        --to;
        graph.add_edge(from, to);
        minimum_predecessor[to] = min(minimum_predecessor[to], from);
    }

    vector<int> begins(n);
    for (int value : minimum_predecessor) ++begins[value];
    const auto reachable = incremental_reachability(graph, 0);
    int one_edge_reachable = 0;
    for (int k = 0; k < n; ++k) {
        one_edge_reachable += begins[k];
        if (reachable.count[k] != k + 1) cout << -1 << '\n';
        else cout << one_edge_reachable - (k + 1) << '\n';
    }
}
