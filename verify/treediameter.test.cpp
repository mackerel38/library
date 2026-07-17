#define PROBLEM "https://judge.yosupo.jp/problem/tree_diameter"

#include "graph/graph.hpp"
#include "tree/diameter.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    undirected_graph<long long> graph(n);
    for (int index = 1; index < n; ++index) {
        int left, right;
        long long cost;
        cin >> left >> right >> cost;
        graph.add_edge(left, right, cost);
    }
    const auto result = tree_diameter(graph);
    cout << result.distance << ' ' << result.path.size() << '\n';
    for (int vertex : result.path) cout << vertex << ' ';
    cout << '\n';
}
