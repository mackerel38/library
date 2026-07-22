#include <bits/stdc++.h>
#include "graph/paritywalk.hpp"

using namespace std;
using namespace poe;

int main() {
    undirected_graph graph(3);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    const auto walk = vertex_parity_walk(graph, vector<int>{1, 0, 1});
    vector<int> parity(3);
    for (const int vertex : walk) parity[vertex] ^= 1;
    assert((parity == vector<int>{1, 0, 1}));
    assert(static_cast<int>(walk.size()) < 12);
}
