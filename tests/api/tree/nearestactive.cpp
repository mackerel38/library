#include "recipe/nearestactive.hpp"

int main() {
    poe::undirected_graph<long long> graph(5);
    graph.add_edge(0, 1, 2);
    graph.add_edge(1, 2, 3);
    graph.add_edge(1, 3, 3);
    graph.add_edge(3, 4, 1);
    poe::nearestactive active(graph);
    assert(!active.nearest(0));
    assert(active.add(2));
    assert(!active.add(2));
    assert(active.distance(4) == 7);
    active.add(0);
    const auto tied = active.nearest(1);
    assert(tied && tied->distance == 2 && tied->vertex == 0);
    assert(!active.toggle(0) && !active[0]);
    assert(active.set(4, true) && active[4]);
    assert(active.nearest(3)->vertex == 4);
    assert(active.remove(2) && !active.remove(2));
}
