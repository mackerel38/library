#include <bits/stdc++.h>
#include "graph/thresholdcomponents.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    undirected_graph<int> graph(3);
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 2);
    const auto count = threshold_component_union_counts<modint998244353>(graph, 2);
    assert((count == vector<modint998244353>{1, 5, 2}));

    undirected_graph<int> isolated(5);
    const auto independent = threshold_component_union_counts<modint998244353>(isolated, 2);
    assert((independent == vector<modint998244353>{1, 5, 10}));
}
