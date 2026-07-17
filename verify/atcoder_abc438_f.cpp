#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/lca.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    undirected_graph<> graph(n);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        graph.add_edge(u, v);
    }

    lcatree tree(graph);
    auto choose2 = [](long long size) { return size * (size + 1) / 2; };
    long long answer = choose2(n);
    for (const auto& edge : graph[0]) {
        const long long component = tree.parent(edge.to) == 0
            ? tree.subtree_size(edge.to)
            : n - tree.subtree_size(0);
        answer -= choose2(component);
    }

    int left = 0, right = 0;
    for (int vertex = 1; vertex < n; ++vertex) {
        if (tree.on_path(right, left, vertex)) {
            right = vertex;
        } else if (tree.on_path(left, vertex, right)) {
            left = vertex;
        } else {
            break;
        }
        answer += 1LL * tree.side_size(left, right) * tree.side_size(right, left);
    }
    cout << answer << '\n';
}
