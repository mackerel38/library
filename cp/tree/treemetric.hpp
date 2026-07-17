#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// O(n^2)。distanceが正重み木の全点間距離ならその木を返し、そうでなければnullopt。
template <class Cost>
std::optional<undirected_graph<Cost>> restore_tree_metric(
    const std::vector<std::vector<Cost>>& distance
) {
    const int n = static_cast<int>(distance.size());
    if (n == 0) return undirected_graph<Cost>(0);
    for (int i = 0; i < n; ++i) {
        if (static_cast<int>(distance[i].size()) != n || distance[i][i] != Cost{})
            return std::nullopt;
        for (int j = 0; j < i; ++j) {
            if (distance[i][j] != distance[j][i] || !(distance[i][j] > Cost{}))
                return std::nullopt;
        }
    }

    undirected_graph<Cost> tree(n);
    for (int vertex = 1; vertex < n; ++vertex) {
        int parent = -1;
        for (int candidate = 0; candidate < n; ++candidate) {
            if (candidate == vertex) continue;
            if (distance[0][candidate] + distance[candidate][vertex] != distance[0][vertex])
                continue;
            if (parent == -1 || distance[candidate][vertex] < distance[parent][vertex])
                parent = candidate;
        }
        if (parent == -1) return std::nullopt;
        tree.add_edge(parent, vertex, distance[parent][vertex]);
    }

    std::vector<Cost> measured(n);
    for (int source = 0; source < n; ++source) {
        std::vector<std::tuple<int, int, Cost>> stack{{source, -1, Cost{}}};
        while (!stack.empty()) {
            auto [vertex, parent, length] = stack.back();
            stack.pop_back();
            measured[vertex] = length;
            for (const auto& current : tree[vertex]) {
                if (current.to != parent)
                    stack.emplace_back(current.to, vertex, length + current.cost);
            }
        }
        if (measured != distance[source]) return std::nullopt;
    }
    return tree;
}

/// O(n^2)。distanceが正重み木の全点間距離ならtrueを返す。
template <class Cost>
bool is_tree_metric(const std::vector<std::vector<Cost>>& distance) {
    return restore_tree_metric(distance).has_value();
}

}
