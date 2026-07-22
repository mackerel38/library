#include <bits/stdc++.h>
#include "tree/pathfold.hpp"

using namespace std;
using namespace poe;

int main() {
    undirected_graph graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    vector<int> values{6, 10, 15, 14};
    auto stats = tree_path_fold_statistics(
        graph, values, [](int left, int right) { return gcd(left, right); });

    assert(stats[2].count == 3 && stats[2].length_sum == 7);
    assert(stats[5].count == 1 && stats[5].length_sum == 2);
    assert(stats[1].count == 2 && stats[1].length_sum == 6);
}
