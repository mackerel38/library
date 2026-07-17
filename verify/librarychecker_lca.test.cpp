#define PROBLEM "https://judge.yosupo.jp/problem/lca"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/rootedtree.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices, queries;
    cin >> vertices >> queries;
    undirected_graph<> graph(vertices);
    for (int vertex = 1; vertex < vertices; ++vertex) {
        int parent;
        cin >> parent;
        graph.add_edge(parent, vertex);
    }
    rootedtree tree(graph, 0);
    vector<vector<int>> up(bit_width(static_cast<unsigned int>(vertices)), vector<int>(vertices));
    for (int vertex = 0; vertex < vertices; ++vertex) up[0][vertex] = max(0, tree.parent(vertex));
    for (int level = 1; level < static_cast<int>(up.size()); ++level) {
        for (int vertex = 0; vertex < vertices; ++vertex) up[level][vertex] = up[level - 1][up[level - 1][vertex]];
    }
    while (queries--) {
        int left, right;
        cin >> left >> right;
        if (tree.depth(left) < tree.depth(right)) swap(left, right);
        int difference = tree.depth(left) - tree.depth(right);
        for (int level = 0; difference; ++level, difference >>= 1) if (difference & 1) left = up[level][left];
        if (left != right) {
            for (int level = static_cast<int>(up.size()) - 1; level >= 0; --level) {
                if (up[level][left] != up[level][right]) left = up[level][left], right = up[level][right];
            }
            left = tree.parent(left);
        }
        cout << left << '\n';
    }
}
