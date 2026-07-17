#define PROBLEM "https://atcoder.jp/contests/abc460/tasks/abc460_g"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/monochromatic.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, query_count;
    cin >> n >> query_count;
    vector<long long> weight(n);
    vector<int> color(n);
    for (auto& value : weight) cin >> value;
    for (int& value : color) cin >> value;
    undirected_graph<> graph(n);
    for (int i = 0; i + 1 < n; ++i) {
        int u, v;
        cin >> u >> v;
        graph.add_edge(--u, --v);
    }

    monochromatictree data(graph, weight, color);
    while (query_count--) {
        int type, vertex;
        cin >> type >> vertex;
        --vertex;
        if (type == 1) {
            data.flip(vertex);
        } else if (type == 2) {
            long long value;
            cin >> value;
            data.add(vertex, value);
        } else {
            cout << data.sum(vertex) << '\n';
        }
    }
}
