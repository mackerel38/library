#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// O(3^n)時間・O(2^n)領域。各辺を二方向または同一視にし、縮約後がDAGとなる割当数を返す。
/// count_acyclic_edge_states<T>(graph): nは小さいこと。Tは加減算と0,1からの構築に対応すること。
template<class T, undirected_graph_type Graph>
T count_acyclic_edge_states(const Graph& graph) {
    const int n = graph.size();
    assert(n <= 22);
    const int subsets = 1 << n;
    std::vector<unsigned int> adjacency(n);
    for (const auto& edge : graph.edges()) {
        assert(edge.from != edge.to);
        adjacency[edge.from] |= 1U << edge.to;
        adjacency[edge.to] |= 1U << edge.from;
    }
    std::vector<int> components(subsets);
    for (int mask = 1; mask < subsets; ++mask) {
        unsigned int remaining = static_cast<unsigned int>(mask);
        while (remaining != 0) {
            ++components[mask];
            unsigned int frontier = remaining & -remaining;
            unsigned int visited = 0;
            while (frontier != 0) {
                visited |= frontier;
                unsigned int neighbors = 0;
                for (unsigned int vertices = frontier;
                     vertices != 0; vertices &= vertices - 1) {
                    neighbors |= adjacency[std::countr_zero(vertices)];
                }
                frontier = neighbors & static_cast<unsigned int>(mask) & ~visited;
            }
            remaining &= ~visited;
        }
    }

    std::vector<T> dp(subsets);
    dp[0] = T{1};
    for (int mask = 1; mask < subsets; ++mask) {
        for (int removed = mask; removed != 0; removed = (removed - 1) & mask) {
            if (components[removed] & 1) {
                dp[mask] += dp[mask ^ removed];
            } else {
                dp[mask] -= dp[mask ^ removed];
            }
        }
    }
    return dp.back();
}

}
