#include <bits/stdc++.h>
#include "tree/demand.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> values(n);
    for (int vertex = 1; vertex < n; ++vertex) cin >> values[vertex];
    undirected_graph graph(n);
    graph.read(n - 1);
    cout << maximum_tree_demand_threshold(graph, values).value_or(0) << '\n';
}
