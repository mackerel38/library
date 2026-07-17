#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/maximumclique.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    undirected_graph graph(n);
    while (m--) {
        int first, second;
        cin >> first >> second;
        graph.add_edge(first - 1, second - 1);
    }
    cout << maximum_clique(graph).size() << '\n';
}
