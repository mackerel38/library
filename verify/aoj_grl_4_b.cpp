#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_4_B"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/topologicalsort.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices, edges;
    cin >> vertices >> edges;
    directed_graph<> graph(vertices);
    while (edges--) {
        int from, to;
        cin >> from >> to;
        graph.add_edge(from, to);
    }
    for (const int vertex : topological_sort(graph).order) cout << vertex << '\n';
}
