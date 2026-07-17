#define PROBLEM "https://atcoder.jp/contests/abc304/tasks/abc304_h"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/topologicalsort.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    directed_graph graph(n);
    for (int edge = 0; edge < m; ++edge) {
        int from, to;
        cin >> from >> to;
        graph.add_edge(from - 1, to - 1);
    }
    vector<int> lower(n), upper(n);
    for (int vertex = 0; vertex < n; ++vertex) {
        cin >> lower[vertex] >> upper[vertex];
        --lower[vertex];
        --upper[vertex];
    }
    const auto result = bounded_topological_sort(graph, lower, upper);
    if (!result.found()) {
        cout << "No\n";
        return 0;
    }
    cout << "Yes\n";
    for (int vertex = 0; vertex < n; ++vertex) {
        cout << result.position[vertex] + 1 << " \n"[vertex + 1 == n];
    }
}
