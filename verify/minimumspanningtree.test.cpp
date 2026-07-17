#define PROBLEM "https://judge.yosupo.jp/problem/minimum_spanning_tree"

#include "graph/graph.hpp"
#include "graph/minimumspanningtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    undirected_graph<long long> graph(n);
    for (int index = 0; index < m; ++index) {
        int left, right;
        long long cost;
        cin >> left >> right >> cost;
        graph.add_edge(left, right, cost);
    }
    const auto result = minimum_spanning_tree(graph);
    cout << result.cost << '\n';
    for (int id : result.edge_ids) cout << id << ' ';
    cout << '\n';
}
