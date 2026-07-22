#include "graph/bottleneck.hpp"
#include "graph/graph.hpp"

using namespace std;
using namespace poe;

int main() {
    directed_graph<> directed(4);
    directed.add_edge(0, 1);
    directed.add_edge(1, 2);
    directed.add_edge(3, 2);
    const auto vertex_answer = minimum_vertex_bottleneck_paths(
        directed, vector<int>{1, 2, 3, 4}, vector<pair<int, int>>{{0, 2}, {2, 0}}
    );
    assert(vertex_answer[0] == 3);
    assert(!vertex_answer[1].has_value());

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

    const vector<terminalhopquery<int>> rest_queries{{0, 2, 2}, {0, 2, 3}};
    assert((terminal_hop_connectivity(graph, vector<int>{0, 2}, rest_queries)
            == vector<bool>{false, true}));
}
