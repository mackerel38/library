#include <algorithm>
#include <cassert>
#include <vector>
#include "graph/graph.hpp"
#include "graph/maximumclique.hpp"

using namespace std;
using namespace poe;

int main() {
    undirected_graph graph(6);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 4);

    auto clique = maximum_clique(graph);
    assert(clique.size() == 3);
    sort(clique.vertices.begin(), clique.vertices.end());
    assert(clique.vertices == vector<int>({0, 1, 2}));

    auto independent = maximum_independent_set(graph);
    assert(independent.size() == 3);
    for (int i = 0; i < independent.size(); ++i) {
        for (int j = i + 1; j < independent.size(); ++j) {
            const int first = independent.vertices[i];
            const int second = independent.vertices[j];
            assert(ranges::none_of(graph[first], [&](const auto& edge) {
                return edge.to == second;
            }));
        }
    }
}
