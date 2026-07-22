#include <bits/stdc++.h>
#include "graph/shortestpath.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, l;
    cin >> n >> m >> k >> l;
    vector<int> labels(n), sources(l);
    for (int& label : labels) cin >> label;
    for (int& source : sources) {
        cin >> source;
        --source;
    }
    undirected_graph<long long> graph(n);
    graph.read(m);
    const auto distance = distance_to_nearest_different_label_source(
        graph, labels, sources);
    for (int vertex = 0; vertex < n; ++vertex) {
        if (vertex) cout << ' ';
        if (distance[vertex] == numeric_limits<long long>::max()) cout << -1;
        else cout << distance[vertex];
    }
    cout << '\n';
}
