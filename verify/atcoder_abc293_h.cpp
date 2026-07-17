#include <bits/stdc++.h>

#include "recipe/treepathpartition.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    undirected_graph graph(n);
    for (int i = 1; i < n; ++i) {
        int a, b;
        cin >> a >> b;
        graph.add_edge(--a, --b);
    }
    cout << minimum_path_partition_width(graph) << '\n';
}
