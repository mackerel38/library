#include <bits/stdc++.h>
#include "flow/vertexcut.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    undirected_graph<> graph(n);
    graph.read(m);
    vector<long long> costs(n);
    for (auto& cost : costs) cin >> cost;
    const auto result = minimum_vertex_cut(graph, costs, 0, n - 1);
    cout << result.cost << '\n';
    cout << result.vertices.size() << '\n';
    for (const int vertex : result.vertices) cout << vertex + 1 << ' ';
    cout << '\n';
}
