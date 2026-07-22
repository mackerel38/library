#include <bits/stdc++.h>
#include "graph/paritywalk.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    undirected_graph graph(n);
    graph.read(m);
    string target_string;
    cin >> target_string;
    vector<int> target(n);
    for (int vertex = 0; vertex < n; ++vertex) {
        target[vertex] = target_string[vertex] - '0';
    }
    const auto walk = vertex_parity_walk(graph, target);
    cout << walk.size() << '\n';
    for (int index = 0; index < static_cast<int>(walk.size()); ++index) {
        if (index) cout << ' ';
        cout << walk[index] + 1;
    }
    cout << '\n';
}
