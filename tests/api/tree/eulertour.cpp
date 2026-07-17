#include <cassert>
#include <vector>
#include "graph/graph.hpp"
#include "tree/eulertour.hpp"

using namespace std;
using namespace poe;

int main() {
    undirected_graph graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);

    eulertour tour(graph);
    assert(tour.vertices() == vector<int>({0, 1, 3, 1, 4, 1, 0, 2, 0}));
    assert(tour.edges() == vector<int>({0, 2, 2, 3, 3, 0, 1, 1}));
    assert(tour.depths() == vector<int>({0, 1, 2, 1, 2, 1, 0, 1, 0}));
    assert(tour.first(1) == 1 && tour.last(1) == 5);
    assert(tour.subtree(1) == pair(1, 4));
    assert(tour.preorder(tour.index(4)) == 4);
    assert(tour[7] == 2 && tour.edge(7) == 1 && tour.depth(7) == 1);

    undirected_graph empty(0);
    eulertour empty_tour(empty);
    assert(empty_tour.size() == 0 && empty_tour.vertex_count() == 0);
}
