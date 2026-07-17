#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/eulertour.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<pair<int, int>> edges(n - 1);
    for (auto& [from, to] : edges) {
        cin >> from >> to;
        --from;
        --to;
        if (from > to) swap(from, to);
    }
    sort(edges.begin(), edges.end());

    undirected_graph graph(n);
    for (const auto& [from, to] : edges) graph.add_edge(from, to);
    eulertour tour(graph, 0);
    for (int index = 0; index < tour.size(); ++index) {
        if (index != 0) cout << ' ';
        cout << tour[index] + 1;
    }
    cout << '\n';
}
