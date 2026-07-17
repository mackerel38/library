#define PROBLEM "https://atcoder.jp/contests/abc301/tasks/abc301_h"
#include <bits/stdc++.h>
#include "graph/bottleneck.hpp"
#include "graph/graph.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    undirected_graph<int> graph(n);
    for (int edge = 0; edge < m; ++edge) {
        int left, right, weight;
        cin >> left >> right >> weight;
        graph.add_edge(left - 1, right - 1, weight);
    }
    bottlenecksensitivity sensitivity(graph);
    int q;
    cin >> q;
    while (q--) {
        int edge, source, target;
        cin >> edge >> source >> target;
        cout << sensitivity.increases(edge - 1, source - 1, target - 1) << '\n';
    }
}
