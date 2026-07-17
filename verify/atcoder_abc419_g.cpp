#define PROBLEM "https://atcoder.jp/contests/abc419/tasks/abc419_g"
#include <bits/stdc++.h>
#include "graph/simplepath.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    undirected_graph<> graph(n);
    while (m--) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(--left, --right);
    }
    const auto answer = simple_path_length_counts(graph, 0, n - 1);
    for (int length = 1; length < n; ++length) cout << answer[length] << ' ';
    cout << '\n';
}
