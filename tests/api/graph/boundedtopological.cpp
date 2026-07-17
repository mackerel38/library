#include "graph/graph.hpp"
#include "graph/topologicalsort.hpp"

int main() {
    poe::directed_graph graph(4);
    graph.add_edge(0, 2);
    graph.add_edge(1, 2);

    const auto result = poe::bounded_topological_sort(
        graph,
        std::vector<int>{0, 0, 2, 1},
        std::vector<int>{2, 1, 3, 3}
    );
    assert(result.found());
    assert(result.position[0] < result.position[2]);
    assert(result.position[1] < result.position[2]);

    const auto impossible = poe::bounded_topological_sort(
        graph,
        std::vector<int>{2, 0, 0, 0},
        std::vector<int>{2, 3, 1, 3}
    );
    assert(!impossible.found());
}
