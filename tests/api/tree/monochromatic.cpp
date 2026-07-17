#include <cassert>
#include <vector>

#include "graph/graph.hpp"
#include "tree/monochromatic.hpp"

int main() {
    poe::undirected_graph<> graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    poe::monochromatictree data(
        graph, std::vector<long long>{1, 10, 100, 1000}, std::vector<int>{0, 0, 1, 0}
    );
    assert(data.sum(0) == 1011 && data.sum(2) == 100);
    data.flip(2);
    assert(data.sum(2) == 1111);
    data.add(0, 4);
    data.set(3, 7);
    assert(data[0] == 5 && data[3] == 7 && data.sum(1) == 122);
}
