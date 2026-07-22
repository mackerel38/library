#include <cassert>
#include <optional>
#include <vector>
#include "flow/vertexcut.hpp"

int main() {
    poe::undirected_graph<> graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(1, 3);
    graph.add_edge(0, 2);
    graph.add_edge(2, 3);
    const auto result = poe::minimum_vertex_cut(
        graph, std::vector<long long>{0, 4, 7, 0}, 0, 3);
    assert(result.possible);
    assert(result.cost == 11);
    assert((result.vertices == std::vector<int>{1, 2}));

    poe::directed_graph<> directed(3);
    directed.add_edge(0, 1);
    directed.add_edge(1, 2);
    std::vector<std::optional<int>> costs = {std::nullopt, 3, std::nullopt};
    assert(poe::minimum_vertex_cut(directed, costs, 0, 2).cost == 3);
}
