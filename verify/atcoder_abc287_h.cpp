#include <bits/stdc++.h>
#include "graph/bottleneck.hpp"
#include "graph/graph.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    directed_graph<> graph(n);
    graph.read(m);
    int q;
    cin >> q;
    vector<pair<int, int>> queries(q);
    for (auto& [source, target] : queries) {
        cin >> source >> target;
        --source;
        --target;
    }
    vector<int> cost(n);
    iota(cost.begin(), cost.end(), 1);
    const auto answer = minimum_vertex_bottleneck_paths(graph, cost, queries);
    for (const auto& value : answer) cout << (value ? *value : -1) << '\n';
}
