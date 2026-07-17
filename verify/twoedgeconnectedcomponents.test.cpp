#define PROBLEM "https://judge.yosupo.jp/problem/two_edge_connected_components"

#include "graph/graph.hpp"
#include "graph/lowlink.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    undirected_graph<> graph(n);
    for (int index = 0; index < m; ++index) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(left, right);
    }
    const auto result = two_edge_connected_components(graph);
    cout << result.count << '\n';
    for (const auto& group : result.groups) {
        cout << group.size();
        for (int vertex : group) cout << ' ' << vertex;
        cout << '\n';
    }
}
