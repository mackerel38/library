#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/constrainedpath.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_cases;
    cin >> test_cases;
    while (test_cases--) {
        int n;
        cin >> n;
        undirected_graph<> graph(n);
        for (int i = 1; i < n; ++i) {
            int a, b;
            cin >> a >> b;
            graph.add_edge(a - 1, b - 1);
        }
        const int answer = longest_constrained_path(
            graph,
            [&](int vertex) { return graph[vertex].size() >= 3; },
            [&](int vertex) { return graph[vertex].size() >= 4; }
        );
        cout << max(1, answer) << '\n';
    }
}
