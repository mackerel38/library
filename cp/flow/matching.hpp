#pragma once
#include <bits/stdc++.h>
#include "flow/mincost.hpp"

namespace poe {

/// minimum_weight_bipartite_matchingへ渡す重み付き二部辺。
template<class Cost>
struct weightedbipartiteedge {
    int left;
    int right;
    Cost weight;
};

/// O(F(m+n log n))。空matchingも許して重み和が最小の二部matchingの重みを返す。
template<class Cost>
Cost minimum_weight_bipartite_matching(
    int left_size,
    int right_size,
    const std::vector<weightedbipartiteedge<Cost>>& edges
) {
    assert(left_size >= 0 && right_size >= 0);
    Cost offset{};
    for (const auto& edge : edges) {
        assert(0 <= edge.left && edge.left < left_size);
        assert(0 <= edge.right && edge.right < right_size);
        if (edge.weight < Cost{}) offset = std::max(offset, -edge.weight);
    }
    const int source = left_size + right_size;
    const int sink = source + 1;
    costflowgraph<int, Cost> graph(sink + 1);
    for (int left = 0; left < left_size; ++left) graph.add_edge(source, left, 1, Cost{});
    for (int right = 0; right < right_size; ++right) {
        graph.add_edge(left_size + right, sink, 1, Cost{});
    }
    for (const auto& edge : edges) {
        if (edge.weight < Cost{}) {
            graph.add_edge(edge.left, left_size + edge.right, 1, edge.weight + offset);
        }
    }
    Cost answer{};
    for (const auto& [flow, shifted_cost] : mincostslope(graph, source, sink)) {
        answer = std::min(answer, shifted_cost - static_cast<Cost>(flow) * offset);
    }
    return answer;
}

}
