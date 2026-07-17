#include "recipe/nearestactive.hpp"
#include "tree/lca.hpp"

int main() {
    std::mt19937 random(172839);
    for (int trial = 0; trial < 500; ++trial) {
        const int n = 1 + random() % 80;
        poe::undirected_graph<long long> graph(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            graph.add_edge(vertex, random() % vertex, random() % 20);
        }
        poe::nearestactive active(graph);
        poe::lcatree distance(graph);
        std::vector<bool> enabled(n);
        for (int operation = 0; operation < 500; ++operation) {
            const int changed = random() % n;
            enabled[changed] = !enabled[changed];
            assert(active.toggle(changed) == enabled[changed]);
            const int query = random() % n;
            std::optional<poe::nearestactiveresult<long long>> expected;
            for (int vertex = 0; vertex < n; ++vertex) {
                if (!enabled[vertex]) continue;
                const long long candidate_distance = distance.distance(query, vertex);
                if (!expected || candidate_distance < expected->distance ||
                    (candidate_distance == expected->distance &&
                     vertex < expected->vertex)) {
                    expected = {{vertex, candidate_distance}};
                }
            }
            const auto actual = active.nearest(query);
            assert(static_cast<bool>(actual) == static_cast<bool>(expected));
            if (actual) {
                assert(actual->vertex == expected->vertex);
                assert(actual->distance == expected->distance);
            }
        }
    }
}
