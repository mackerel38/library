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

namespace detail {

template<class Cost, bool Maximum>
std::vector<Cost> weighted_bipartite_matching_values(
    int left_size,
    int right_size,
    const std::vector<weightedbipartiteedge<Cost>>& edges
) {
    assert(left_size >= 0 && right_size >= 0);
    Cost offset{};
    for (const auto& edge : edges) {
        assert(0 <= edge.left && edge.left < left_size);
        assert(0 <= edge.right && edge.right < right_size);
        if constexpr (Maximum) offset = std::max(offset, edge.weight);
        else if (edge.weight < Cost{}) offset = std::max(offset, -edge.weight);
    }
    const int source = left_size + right_size;
    const int sink = source + 1;
    costflowgraph<int, Cost> graph(sink + 1);
    for (int left = 0; left < left_size; ++left) {
        graph.add_edge(source, left, 1, Cost{});
    }
    for (int right = 0; right < right_size; ++right) {
        graph.add_edge(left_size + right, sink, 1, Cost{});
    }
    for (const auto& edge : edges) {
        const Cost cost = Maximum ? offset - edge.weight : offset + edge.weight;
        graph.add_edge(edge.left, left_size + edge.right, 1, cost);
    }

    const auto slope = mincostslope(graph, source, sink);
    std::vector<Cost> result(1, Cost{});
    for (int index = 1; index < static_cast<int>(slope.size()); ++index) {
        const auto [previous_flow, previous_cost] = slope[index - 1];
        const auto [next_flow, next_cost] = slope[index];
        const Cost marginal =
            (next_cost - previous_cost) / static_cast<Cost>(next_flow - previous_flow);
        for (int flow = previous_flow + 1; flow <= next_flow; ++flow) {
            const Cost shifted = previous_cost
                               + marginal * static_cast<Cost>(flow - previous_flow);
            if constexpr (Maximum) {
                result.push_back(static_cast<Cost>(flow) * offset - shifted);
            } else {
                result.push_back(shifted - static_cast<Cost>(flow) * offset);
            }
        }
    }
    return result;
}

}  // namespace detail

/// O(F(m+n log n))。matchingサイズkごとの最小重みをk=0..Fの順に返す。
template<class Cost>
std::vector<Cost> minimum_weight_bipartite_matching_values(
    int left_size,
    int right_size,
    const std::vector<weightedbipartiteedge<Cost>>& edges
) {
    return detail::weighted_bipartite_matching_values<Cost, false>(
        left_size, right_size, edges
    );
}

/// O(F(m+n log n))。matchingサイズkごとの最大重みをk=0..Fの順に返す。
template<class Cost>
std::vector<Cost> maximum_weight_bipartite_matching_values(
    int left_size,
    int right_size,
    const std::vector<weightedbipartiteedge<Cost>>& edges
) {
    return detail::weighted_bipartite_matching_values<Cost, true>(
        left_size, right_size, edges
    );
}

/// O(F(m+n log n))。空matchingも許して重み和が最小の二部matchingの重みを返す。
template<class Cost>
Cost minimum_weight_bipartite_matching(
    int left_size,
    int right_size,
    const std::vector<weightedbipartiteedge<Cost>>& edges
) {
    const auto values = minimum_weight_bipartite_matching_values(
        left_size, right_size, edges
    );
    return *std::min_element(values.begin(), values.end());
}

}
