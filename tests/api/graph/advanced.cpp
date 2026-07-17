#include "graph/cycle.hpp"
#include "graph/warshallfloyd.hpp"

int main() {
    poe::directed_graph<> directed(4);
    directed.add_edge(0, 1);
    directed.add_edge(1, 2);
    directed.add_edge(2, 0);
    directed.add_edge(2, 3);
    const auto directed_cycle = poe::find_cycle(directed);
    assert(directed_cycle.found());
    assert(directed_cycle.vertices.size() == directed_cycle.edge_ids.size());

    poe::undirected_graph<> undirected(3);
    undirected.add_edge(0, 1);
    undirected.add_edge(0, 1);
    const auto undirected_cycle = poe::find_cycle(undirected);
    assert(undirected_cycle.found() && undirected_cycle.edge_ids.size() == 2);

    poe::directed_graph<long long> weighted(4);
    weighted.add_edge(0, 1, 4);
    weighted.add_edge(0, 2, 10);
    weighted.add_edge(1, 2, -2);
    weighted.add_edge(2, 3, 3);
    const auto shortest = poe::warshall_floyd(weighted);
    assert(shortest(0, 3) == 5);
    assert(shortest.path(0, 3) == std::vector<int>({0, 1, 2, 3}));
    assert(!shortest.has_negative_cycle());
    assert(!shortest.reachable(3, 0));
}
