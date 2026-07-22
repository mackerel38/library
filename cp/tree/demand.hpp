#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// O(n)。dp[v]=demand[v]+max(sum(dp[child])-allowance[v],0)を根付き木上で返す。
template<undirected_graph_type Graph, class T>
std::vector<T> tree_demand_residual(
    const Graph& graph,
    const std::vector<T>& demand,
    const std::vector<T>& allowance,
    int root = 0
) {
    const int n = graph.size();
    assert(static_cast<int>(demand.size()) == n);
    assert(static_cast<int>(allowance.size()) == n);
    assert(n == 0 || (0 <= root && root < n));
    assert(n == 0 || graph.edge_count() == n - 1);
    for (int vertex = 0; vertex < n; ++vertex) {
        assert(demand[vertex] >= T{} && allowance[vertex] >= T{});
    }
    if (n == 0) return {};

    std::vector<int> parent(n, -2), order;
    order.reserve(n);
    parent[root] = -1;
    order.push_back(root);
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

    std::vector<T> residual = demand;
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int vertex = *iterator;
        T children{};
        for (const auto& edge : graph[vertex]) {
            if (parent[edge.to] == vertex) children += residual[edge.to];
        }
        if (children > allowance[vertex]) {
            residual[vertex] += children - allowance[vertex];
        }
    }
    return residual;
}

/// O(n log n)。値がthreshold以上の非根頂点を需要1としたとき根の残余が正となる最大値を返す。
template<undirected_graph_type Graph, class Value>
std::optional<Value> maximum_tree_demand_threshold(
    const Graph& graph,
    const std::vector<Value>& values,
    int root = 0,
    long long allowance_per_vertex = 1
) {
    const int n = graph.size();
    assert(static_cast<int>(values.size()) == n);
    assert(n == 0 || (0 <= root && root < n));
    assert(allowance_per_vertex >= 0);
    if (n <= 1) return std::nullopt;

    std::vector<Value> candidates;
    candidates.reserve(n - 1);
    for (int vertex = 0; vertex < n; ++vertex) {
        if (vertex != root) candidates.push_back(values[vertex]);
    }
    std::sort(candidates.begin(), candidates.end());
    candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());
    std::vector<long long> demand(n), allowance(n, allowance_per_vertex);
    const auto feasible = [&](const Value& threshold) {
        for (int vertex = 0; vertex < n; ++vertex) {
            demand[vertex] = vertex != root && values[vertex] >= threshold;
        }
        return tree_demand_residual(graph, demand, allowance, root)[root] > 0;
    };

    int left = 0, right = static_cast<int>(candidates.size());
    while (left < right) {
        const int middle = (left + right) / 2;
        if (feasible(candidates[middle])) left = middle + 1;
        else right = middle;
    }
    if (left == 0) return std::nullopt;
    return candidates[left - 1];
}

}  // namespace poe
