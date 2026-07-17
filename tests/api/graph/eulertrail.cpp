#include "graph/eulertrail.hpp"

int main() {
    poe::directed_graph<> directed(4);
    directed.add_edge(0, 1);
    directed.add_edge(1, 2);
    directed.add_edge(2, 0);
    directed.add_edge(0, 3);
    auto first = poe::euler_trail(directed);
    assert(first && first.vertex_path.front() == 0 && first.vertex_path.back() == 3);
    assert(first.edge_path.size() == 4);
    assert(!poe::euler_trail(directed, 1));

    poe::undirected_graph<> undirected(3);
    undirected.add_edge(0, 1);
    undirected.add_edge(1, 2);
    undirected.add_edge(2, 0);
    undirected.add_edge(0, 0);
    auto second = poe::euler_trail(undirected, 1);
    assert(second && second.edge_path.size() == 4);

    poe::undirected_graph<> disconnected(4);
    disconnected.add_edge(0, 1);
    disconnected.add_edge(2, 3);
    assert(!poe::euler_trail(disconnected));
}
