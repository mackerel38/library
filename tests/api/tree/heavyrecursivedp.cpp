#include "graph/graph.hpp"
#include "tree/heavyrecursivedp.hpp"

int main() {
    poe::undirected_graph graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    std::vector<std::array<int, 2>> recorded(4);
    const auto result = poe::heavyrecursivedp<2>(
        graph,
        0,
        0,
        [](int vertex, auto& states) {
            states[0] += vertex + 1;
            states[1] += 2 * (vertex + 1);
        },
        [&](int vertex, const auto& states) { recorded[vertex] = states; });
    assert(result == recorded[0]);
    assert(recorded[2] == (std::array<int, 2>{3, 6}));
}

