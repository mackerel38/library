#define PROBLEM "https://atcoder.jp/contests/abc306/tasks/abc306_h"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/orientation.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    undirected_graph graph(n);
    for (int edge = 0; edge < m; ++edge) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(left - 1, right - 1);
    }
    cout << count_acyclic_edge_states<modint998244353>(graph) << '\n';
}

