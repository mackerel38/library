#define PROBLEM "https://atcoder.jp/contests/abc298/tasks/abc298_h"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/treedistancesum.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    undirected_graph graph(n);
    for (int edge = 1; edge < n; ++edge) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(left - 1, right - 1);
    }
    treedistancesum distances(graph);
    int q;
    cin >> q;
    while (q--) {
        int left, right;
        cin >> left >> right;
        cout << distances.min_sum(left - 1, right - 1) << '\n';
    }
}
