#include <bits/stdc++.h>
#include "graph/thresholdcomponents.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    undirected_graph<int> graph(n);
    graph.read(m);
    const auto count = threshold_component_union_counts<modint998244353>(graph, k);
    cout << accumulate(count.begin(), count.end(), modint998244353{}) << '\n';
}
