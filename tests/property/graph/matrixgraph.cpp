#include "graph/matrixgraph.hpp"

int main() {
    std::mt19937 random(20260717);

    for (int n = 0; n <= 12; ++n) {
        for (int iteration = 0; iteration < 200; ++iteration) {
            poe::directed_graph<int> list(n);
            std::vector<std::vector<char>> present(n, std::vector<char>(n));
            std::vector<std::vector<int>> cost(n, std::vector<int>(n));
            for (int from = 0; from < n; ++from) {
                for (int to = 0; to < n; ++to) {
                    if (random() % 4 != 0) continue;
                    present[from][to] = true;
                    cost[from][to] = static_cast<int>(random() % 101) - 50;
                    list.add_edge(from, to, cost[from][to]);
                }
            }

            const poe::directed_matrix_graph<int> matrix(list);
            const poe::directed_graph<int> restored(matrix);
            assert(matrix.edge_count() == list.edge_count());
            assert(restored.edge_count() == list.edge_count());
            for (int from = 0; from < n; ++from) {
                for (int to = 0; to < n; ++to) {
                    assert(matrix.has_edge(from, to) == static_cast<bool>(present[from][to]));
                    if (present[from][to]) assert(matrix.cost(from, to) == cost[from][to]);
                }
            }
        }
    }

    for (int n = 0; n <= 12; ++n) {
        for (int iteration = 0; iteration < 200; ++iteration) {
            poe::undirected_matrix_graph<> matrix(n);
            for (int from = 0; from < n; ++from) {
                for (int to = from; to < n; ++to) {
                    if (random() % 5 == 0) matrix.set_edge(from, to);
                }
            }
            const poe::undirected_graph<> list(matrix);
            const poe::undirected_matrix_graph<> restored(list);
            assert(list.edge_count() == matrix.edge_count());
            assert(restored.edge_count() == matrix.edge_count());
            for (int from = 0; from < n; ++from) {
                for (int to = 0; to < n; ++to) {
                    assert(restored.has_edge(from, to) == matrix.has_edge(from, to));
                }
            }
        }
    }
}
