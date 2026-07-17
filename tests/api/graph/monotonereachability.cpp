#include <cassert>
#include "graph/monotonereachability.hpp"

int main() {
    poe::directed_graph<> graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(3, 4);
    poe::monotonereachability reached(graph);
    reached.add_source(0);
    assert(reached.contains(2));
    assert(!reached.contains(4));
    reached.add_source(3);
    assert(reached.count() == 5);
}
