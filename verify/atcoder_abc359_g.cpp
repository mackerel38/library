#include <bits/stdc++.h>
#include "tree/virtualtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    undirected_graph<> graph(n);
    for (int edge = 0; edge + 1 < n; ++edge) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(left - 1, right - 1);
    }
    vector<vector<int>> groups(n + 1);
    for (int vertex = 0; vertex < n; ++vertex) {
        int value;
        cin >> value;
        groups[value].push_back(vertex);
    }
    virtualtree builder(graph);
    long long answer = 0;
    for (const auto& vertices : groups) {
        if (vertices.size() < 2) continue;
        const auto tree = builder.build(vertices);
        vector<int> count(tree.size());
        for (int node = tree.size() - 1; node >= 0; --node) {
            count[node] += tree.selected[node];
            const int parent = tree.parent[node];
            if (parent == -1) continue;
            answer += 1LL * tree.edge_length[node] * count[node] *
                      (static_cast<int>(vertices.size()) - count[node]);
            count[parent] += count[node];
        }
    }
    cout << answer << '\n';
}
