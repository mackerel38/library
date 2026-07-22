#pragma once
#include <bits/stdc++.h>
#include "math/gaussian.hpp"
#include "math/matrix.hpp"

namespace poe {

/// O(2^n n^3 + 3^n n)時間・O(2^n n)領域。重み付き無向グラフの森の重み和を辺数別に返す。
template<class T>
std::vector<T> spanning_forest_counts(const matrix<T>& edge_weight) {
    assert(edge_weight.height() == edge_weight.width());
    const int vertices = edge_weight.height();
    assert(vertices < std::numeric_limits<unsigned>::digits);
    for (int vertex = 0; vertex < vertices; ++vertex) {
        assert(edge_weight[vertex][vertex] == T{});
        for (int other = 0; other < vertex; ++other) {
            assert(edge_weight[vertex][other] == edge_weight[other][vertex]);
        }
    }
    if (vertices == 0) return {T{1}};

    const int subsets = 1 << vertices;
    std::vector<T> tree_weight(subsets);
    for (int mask = 1; mask < subsets; ++mask) {
        std::vector<int> selected;
        for (int vertex = 0; vertex < vertices; ++vertex) {
            if (mask >> vertex & 1) selected.push_back(vertex);
        }
        if (selected.size() == 1) {
            tree_weight[mask] = T{1};
            continue;
        }

        const int minor_size = static_cast<int>(selected.size()) - 1;
        matrix<T> laplacian(minor_size, minor_size);
        for (int row = 0; row < minor_size; ++row) {
            const int vertex = selected[row];
            for (int other : selected) {
                if (other != vertex) laplacian[row][row] += edge_weight[vertex][other];
            }
            for (int column = 0; column < minor_size; ++column) {
                if (row != column) {
                    laplacian[row][column] -= edge_weight[vertex][selected[column]];
                }
            }
        }
        tree_weight[mask] = determinant(std::move(laplacian));
    }

    std::vector<std::vector<T>> partitions(subsets, std::vector<T>(vertices + 1));
    partitions[0][0] = T{1};
    for (int mask = 1; mask < subsets; ++mask) {
        const int root = std::countr_zero(static_cast<unsigned>(mask));
        const int root_bit = 1 << root;
        const int others = mask ^ root_bit;
        for (int submask = others;; submask = (submask - 1) & others) {
            const int component = submask | root_bit;
            const int remaining = mask ^ component;
            if (tree_weight[component] != T{}) {
                for (int count = 0; count < vertices; ++count) {
                    partitions[mask][count + 1]
                        += tree_weight[component] * partitions[remaining][count];
                }
            }
            if (submask == 0) break;
        }
    }

    std::vector<T> result(vertices);
    const int full = subsets - 1;
    for (int components = 1; components <= vertices; ++components) {
        result[vertices - components] = partitions[full][components];
    }
    return result;
}

}
