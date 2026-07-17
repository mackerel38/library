#include "graph/bottleneck.hpp"
#include "graph/graph.hpp"

using namespace std;
using namespace poe;

int main() {
    undirected_graph<int> graph(4);
    const int first = graph.add_edge(0, 1, 1);
    const int second = graph.add_edge(1, 2, 2);
    const int direct = graph.add_edge(0, 2, 3);
    const int bridge = graph.add_edge(2, 3, 4);
    bottlenecksensitivity sensitivity(graph);
    assert(sensitivity.critical(first));
    assert(sensitivity.increases(second, 0, 2));
    assert(!sensitivity.increases(first, 0, 2));
    assert(!sensitivity.increases(direct, 0, 2));
    assert(sensitivity.increases(bridge, 0, 3));

    undirected_graph<int> parallel(2);
    parallel.add_edge(0, 1, 5);
    parallel.add_edge(0, 1, 5);
    bottlenecksensitivity duplicate(parallel);
    assert(!duplicate.increases(0, 0, 1));
    assert(!duplicate.increases(1, 0, 1));
}
