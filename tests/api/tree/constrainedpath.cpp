#include <cassert>

#include "graph/graph.hpp"
#include "tree/constrainedpath.hpp"

int main() {
    poe::undirected_graph<> graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(2, 4);
    const int answer = poe::longest_constrained_path(
        graph,
        [](int vertex) { return vertex != 4; },
        [](int vertex) { return vertex == 1 || vertex == 2; }
    );
    assert(answer == 4);
}
