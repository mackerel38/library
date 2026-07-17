#include "graph/graph.hpp"
#include "tree/treedistancesum.hpp"

using namespace std;
using namespace poe;

int main() {
    undirected_graph path(5);
    for (int vertex = 1; vertex < 5; ++vertex) path.add_edge(vertex - 1, vertex);
    treedistancesum distances(path);
    assert(distances.sum(0) == 10);
    assert(distances.sum(2) == 6);
    assert(distances.subtree_sum(4, 2) == 3);
    assert(distances.subtree_sum(0, 2) == 9);
    assert(distances.min_sum(0, 4) == 4);
    assert(distances.min_sum(2, 2) == 6);
}
