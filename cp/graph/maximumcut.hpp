#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// 最大cutの重みと二色分け。side[v]が異なる辺がcutへ入る。
template <class Weight>
struct maximumcutresult {
    Weight weight{};
    std::vector<bool> side;
};

/// O(2^(n-1)m)。小頂点数の非負重み無向グラフで最大cutと二色分けを返す。
template <undirected_graph_type Graph>
auto maximum_cut(const Graph& graph) {
    using weight_type = std::conditional_t<
        Graph::is_weighted,
        typename Graph::cost_type,
        long long
    >;
    const int size = graph.size();
    assert(size < 64);
    maximumcutresult<weight_type> result;
    result.side.assign(size, false);
    if (size == 0) return result;
    const std::uint64_t count = std::uint64_t{1} << (size - 1);
    for (std::uint64_t mask = 0; mask < count; ++mask) {
        weight_type weight{};
        for (const auto& edge : graph.edges()) {
            const bool from = edge.from == 0 ? false : static_cast<bool>(mask >> (edge.from - 1) & 1);
            const bool to = edge.to == 0 ? false : static_cast<bool>(mask >> (edge.to - 1) & 1);
            if (from == to) continue;
            if constexpr (Graph::is_weighted) {
                assert(edge.cost >= weight_type{});
                weight += edge.cost;
            } else {
                ++weight;
            }
        }
        if (weight <= result.weight) continue;
        result.weight = weight;
        for (int vertex = 1; vertex < size; ++vertex) {
            result.side[vertex] = mask >> (vertex - 1) & 1;
        }
    }
    return result;
}

}
