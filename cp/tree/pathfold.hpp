#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// O(1)で参照できる、同じ畳み込み値を持つ木上パスの本数と頂点数で測った長さの総和。
struct treepathfoldstat {
    long long count = 0;
    long long length_sum = 0;
};

/// O(Σ ab(cost(combine)+log S))。全ての相異なる二頂点間パスを畳み込み結果ごとに集計する。
template<undirected_graph_type Graph, class Value, class Combine,
         class Compare = std::less<Value>>
std::map<Value, treepathfoldstat, Compare> tree_path_fold_statistics(
    const Graph& graph, const std::vector<Value>& values, Combine combine,
    Compare compare = Compare()) {
    const int n = graph.size();
    assert(static_cast<int>(values.size()) == n);
    assert(n == 0 || graph.edge_count() == n - 1);

    std::map<Value, treepathfoldstat, Compare> answer(compare);
    if (n == 0) return answer;

    std::vector<int> parent(n, -2), order;
    order.reserve(n);
    parent[0] = -1;
    order.push_back(0);
    for (std::size_t index = 0; index < order.size(); ++index) {
        const int vertex = order[index];
        for (const auto& edge : graph[vertex]) {
            if (edge.to == parent[vertex]) continue;
            assert(parent[edge.to] == -2);
            parent[edge.to] = vertex;
            order.push_back(edge.to);
        }
    }
    assert(static_cast<int>(order.size()) == n);

    using map_type = std::map<Value, treepathfoldstat, Compare>;
    std::vector<map_type> subtree;
    subtree.reserve(n);
    for (int vertex = 0; vertex < n; ++vertex) {
        subtree.emplace_back(compare);
    }

    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int vertex = *iterator;
        auto& current = subtree[vertex];
        current[values[vertex]] = {1, 1};

        for (const auto& edge : graph[vertex]) {
            const int child = edge.to;
            if (parent[child] != vertex) continue;
            const auto& child_states = subtree[child];

            for (const auto& [left_value, left] : current) {
                for (const auto& [right_value, right] : child_states) {
                    auto& result = answer[combine(left_value, right_value)];
                    result.count += left.count * right.count;
                    result.length_sum +=
                        left.count * right.length_sum
                        + left.length_sum * right.count;
                }
            }

            for (const auto& [value, state] : child_states) {
                auto& destination = current[combine(values[vertex], value)];
                destination.count += state.count;
                destination.length_sum += state.length_sum + state.count;
            }
            subtree[child].clear();
        }
    }
    return answer;
}

}  // namespace poe
