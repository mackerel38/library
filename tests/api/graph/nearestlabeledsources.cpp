#include <bits/stdc++.h>
#include "graph/shortestpath.hpp"

using namespace std;
using namespace poe;

int main() {
    undirected_graph<long long> graph(4);
    graph.add_edge(0, 1, 15);
    graph.add_edge(1, 2, 30);
    graph.add_edge(2, 3, 40);
    graph.add_edge(0, 3, 10);
    vector<int> labels{0, 0, 1, 1};
    const auto distance = distance_to_nearest_different_label_source(
        graph, labels, vector<int>{1, 2});
    assert((distance == vector<long long>{45, 30, 30, 25}));

    const auto nearest = nearest_labeled_sources_dijkstra<2>(
        graph, vector<pair<int, int>>{{1, 0}, {2, 1}});
    assert(nearest[0][0].source == 1);
    assert(nearest[0][1].source == 2);
}
