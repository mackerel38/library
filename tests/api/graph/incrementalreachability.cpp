#include <cassert>

#include "graph/graph.hpp"
#include "graph/incrementalreachability.hpp"

int main() {
    poe::directed_graph<> graph(5);
    graph.add_edge(0, 2);
    graph.add_edge(2, 1);
    graph.add_edge(1, 4);
    graph.add_edge(4, 3);
    const auto result = poe::incremental_reachability(graph, 0);
    assert((result.count == std::vector<int>{1, 1, 3, 3, 5}));
    assert(result.reached_at[4] == 4);
}
