#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_11_D"
#include <bits/stdc++.h>
#include "graph/connectivity.hpp"
#include "graph/graph.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices, edges;
    cin >> vertices >> edges;
    undirected_graph<> graph(vertices);
    while (edges--) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(left, right);
    }
    const auto components = connected_components(graph);
    int queries;
    cin >> queries;
    while (queries--) {
        int left, right;
        cin >> left >> right;
        cout << (components.same(left, right) ? "yes" : "no") << '\n';
    }
}
