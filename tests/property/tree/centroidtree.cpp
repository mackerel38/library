#include "tree/centroidtree.hpp"
#include "tree/lca.hpp"

int main() {
    std::mt19937 random(917283);
    for (int trial = 0; trial < 2000; ++trial) {
        const int n = 1 + random() % 100;
        poe::undirected_graph<long long> graph(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            graph.add_edge(vertex, random() % vertex, 1 + random() % 20);
        }
        poe::centroidtree decomposition(graph);
        poe::lcatree distances(graph);
        assert(decomposition.order().front() == decomposition.root());
        for (int vertex = 0; vertex < n; ++vertex) {
            const auto& path = decomposition.path(vertex);
            assert(path.back().centroid == vertex && path.back().distance == 0);
            assert(path.size() <= static_cast<std::size_t>(std::bit_width(
                                      static_cast<unsigned>(n))) + 1);
            for (int index = 0; index < static_cast<int>(path.size()); ++index) {
                assert(path[index].distance ==
                       distances.distance(vertex, path[index].centroid));
                if (index == 0) {
                    assert(path[index].centroid == decomposition.root());
                } else {
                    assert(decomposition.parent(path[index].centroid) ==
                           path[index - 1].centroid);
                }
                if (path[index].centroid == vertex) {
                    assert(path[index].branch == -1);
                } else {
                    assert(path[index].branch != -1);
                }
            }
        }
        for (int index = 1; index < n; ++index) {
            assert(decomposition.depth(decomposition[index]) > 0);
        }
    }
}
