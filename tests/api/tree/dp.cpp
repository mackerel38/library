#include <cassert>
#include <vector>

#include "graph/graph.hpp"
#include "tree/dp.hpp"

int main() {
    poe::undirected_graph<> graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);
    const auto size = poe::treedp(
        graph, 0, 0,
        [](int left, int right) { return left + right; },
        [](int children, int) { return children + 1; },
        [](int child, const auto&) { return child; }
    );
    assert(size == std::vector<int>({5, 3, 1, 1, 1}));
}
