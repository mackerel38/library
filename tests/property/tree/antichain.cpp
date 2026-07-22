#include "graph/graph.hpp"
#include "tree/antichain.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(269);
    for (int iteration = 0; iteration < 3000; ++iteration) {
        const int n = 1 + static_cast<int>(random() % 12);
        undirected_graph<> graph(n);
        vector<int> parent(n, -1);
        for (int vertex = 1; vertex < n; ++vertex) {
            parent[vertex] = static_cast<int>(random() % vertex);
            graph.add_edge(parent[vertex], vertex);
        }
        vector<modint998244353> expected(n + 1);
        for (int mask = 0; mask < (1 << n); ++mask) {
            bool valid = true;
            for (int vertex = 0; vertex < n; ++vertex) {
                if (!(mask >> vertex & 1)) continue;
                for (int ancestor = parent[vertex]; ancestor != -1; ancestor = parent[ancestor]) {
                    if (mask >> ancestor & 1) valid = false;
                }
            }
            if (valid) ++expected[std::popcount(static_cast<unsigned int>(mask))];
        }
        assert(tree_antichain_counts(graph) == expected);

        const int root = static_cast<int>(random() % n);
        vector<int> rooted_parent(n, -2), order{root};
        rooted_parent[root] = -1;
        for (size_t index = 0; index < order.size(); ++index) {
            for (const auto& edge : graph[order[index]]) {
                if (rooted_parent[edge.to] != -2) continue;
                rooted_parent[edge.to] = order[index];
                order.push_back(edge.to);
            }
        }
        fill(expected.begin(), expected.end(), 0);
        for (int mask = 0; mask < (1 << n); ++mask) {
            bool valid = true;
            for (int vertex = 0; vertex < n; ++vertex) {
                if (!(mask >> vertex & 1)) continue;
                for (int ancestor = rooted_parent[vertex]; ancestor != -1;
                     ancestor = rooted_parent[ancestor]) {
                    if (mask >> ancestor & 1) valid = false;
                }
            }
            if (valid) ++expected[std::popcount(static_cast<unsigned int>(mask))];
        }
        assert(tree_antichain_counts(graph, root) == expected);
    }
}
