#include <bits/stdc++.h>
#include "tree/demand.hpp"

using namespace std;
using namespace poe;

int main() {
    undirected_graph graph(7);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);
    graph.add_edge(4, 5);
    graph.add_edge(4, 6);
    vector<int> values{0, 2, 4, 6, 5, 6, 10};
    assert(maximum_tree_demand_threshold(graph, values) == 5);

    vector<long long> demand{0, 0, 0, 1, 1, 1, 1};
    vector<long long> allowance(7, 1);
    const auto residual = tree_demand_residual(graph, demand, allowance);
    assert(residual[0] == 1);
}
