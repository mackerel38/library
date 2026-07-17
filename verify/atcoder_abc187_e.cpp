#include <bits/stdc++.h>
#include "tree/difference.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    undirected_graph<> graph(n);
    vector<pair<int, int>> edges(n - 1);
    for (auto& [left, right] : edges) {
        cin >> left >> right;
        --left;
        --right;
        graph.add_edge(left, right);
    }
    subtreedifference<decltype(graph), long long> difference(graph);
    int q;
    cin >> q;
    while (q--) {
        int type, edge;
        long long value;
        cin >> type >> edge >> value;
        --edge;
        int from = edges[edge].first;
        int to = edges[edge].second;
        if (type == 2) swap(from, to);
        if (difference.parent(from) == to) difference.add(from, value);
        else difference.add_complement(to, value);
    }
    difference.build();
    for (long long value : difference.values()) cout << value << '\n';
}
