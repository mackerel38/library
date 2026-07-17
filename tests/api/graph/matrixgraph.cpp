#include "graph/matrixgraph.hpp"

int main() {
    constexpr long long inf = (1LL << 60);

    poe::directed_graph<long long> list(3);
    list.add_edge(0, 1, 7);
    list.add_edge(1, 2, -2);
    list.add_edge(0, 1, 4);

    poe::directed_matrix_graph<long long> matrix(list);
    assert(matrix.size() == 3 && matrix.edge_count() == 2);
    assert(matrix.has_edge(0, 1) && matrix.cost(0, 1) == 4);
    assert(!matrix.has_edge(1, 0));

    poe::directed_graph<long long> restored(matrix);
    assert(restored.size() == 3 && restored.edge_count() == 2);
    assert(restored[0][0].to == 1 && restored[0][0].cost == 4);

    const int added = restored.add_vertex();
    const int first = restored.add_vertices(2);
    assert(added == 3 && first == 4 && restored.size() == 6);
    restored.add_edge(3, 5, 9);

    poe::directed_matrix_graph<long long> from_values(
        std::vector<std::vector<long long>>{
            {inf, 5, inf},
            {inf, inf, 8},
            {1, inf, inf},
        },
        inf
    );
    assert(from_values.edge_count() == 3 && from_values.cost(2, 0) == 1);
    const auto values = from_values.values(inf);
    assert(values[0][1] == 5 && values[1][0] == inf);
    const auto ordinary_matrix = from_values.to_matrix(inf);
    poe::directed_matrix_graph<long long> from_matrix(ordinary_matrix, inf);
    assert(from_matrix.edge_count() == 3 && from_matrix.cost(2, 0) == 1);
    assert(from_values.remove_edge(0, 1));
    assert(!from_values.remove_edge(0, 1));

    poe::undirected_matrix_graph<> plain(
        std::vector<std::vector<int>>{
            {0, 1, 0},
            {1, 0, 1},
            {0, 1, 0},
        }
    );
    assert(plain.edge_count() == 2 && plain.has_edge(1, 0));
    plain.set_edge(0, 2);
    assert(plain.has_edge(2, 0) && plain.edge_count() == 3);
    const int plain_first = plain.add_vertices(2);
    assert(plain_first == 3 && plain.size() == 5);
    plain.set_edge(3, 4);

    poe::undirected_graph<> plain_list(plain);
    assert(plain_list.edge_count() == 4 && plain_list.degree(3) == 1);

    static_assert(poe::matrix_graph_type<poe::matrix_graph<int>>);
    static_assert(poe::matrix_graph_type<poe::undirected_matrix_graph<>>);
}
