#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include "graph/graph.hpp"
#include "tree/lca.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, query_count;
    cin >> n >> query_count;
    undirected_graph<> graph(n);
    for (int vertex = 1; vertex < n; ++vertex) {
        int parent;
        cin >> parent;
        graph.add_edge(parent, vertex);
    }
    lcatree tree(graph, 0);
    while (query_count--) {
        int left, right;
        cin >> left >> right;
        cout << tree.lca(left, right) << '\n';
    }
}
