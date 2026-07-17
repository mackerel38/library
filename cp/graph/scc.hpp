#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

namespace detail {

inline std::pair<int, std::vector<int>> scc_ids_from_adjacency(
    const std::vector<std::vector<int>>& graph,
    const std::vector<std::vector<int>>& reverse_graph) {
    const int n = static_cast<int>(graph.size());
    std::vector<char> used(n);
    std::vector<int> order;
    order.reserve(n);
    for (int vertex = 0; vertex < n; ++vertex) {
        if (used[vertex]) continue;
        used[vertex] = true;
        std::vector<std::pair<int, int>> stack = {{vertex, 0}};
        while (!stack.empty()) {
            auto& [current, edge_index] = stack.back();
            if (edge_index < static_cast<int>(graph[current].size())) {
                const int to = graph[current][edge_index++];
                if (!used[to]) {
                    used[to] = true;
                    stack.emplace_back(to, 0);
                }
                continue;
            }
            order.push_back(current);
            stack.pop_back();
        }
    }
    std::vector<int> ids(n, -1);
    int group_count = 0;
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        if (ids[*iterator] != -1) continue;
        ids[*iterator] = group_count;
        std::vector<int> stack = {*iterator};
        while (!stack.empty()) {
            const int current = stack.back();
            stack.pop_back();
            for (int to : reverse_graph[current]) {
                if (ids[to] != -1) continue;
                ids[to] = group_count;
                stack.push_back(to);
            }
        }
        ++group_count;
    }
    return {group_count, ids};
}

inline std::vector<std::vector<int>> scc_groups_from_ids(
    int group_count, const std::vector<int>& ids) {
    std::vector<int> counts(group_count);
    for (int id : ids) ++counts[id];
    std::vector<std::vector<int>> groups(group_count);
    for (int id = 0; id < group_count; ++id) groups[id].reserve(counts[id]);
    for (int vertex = 0; vertex < static_cast<int>(ids.size()); ++vertex) {
        groups[ids[vertex]].push_back(vertex);
    }
    return groups;
}

}

/// 有向グラフを強連結成分分解する: sccgraph graph(n); 計算O(n+m)。
struct sccgraph {
    /// O(1)。空グラフを作る。
    sccgraph() = default;

    /// O(n)。n頂点のグラフを作る。
    explicit sccgraph(int n) : n_(n) {
        assert(n >= 0);
    }

    /// O(1)。有向辺from->toを追加する。多重辺と自己辺も使える。
    void add_edge(int from, int to) {
        assert_vertex(from);
        assert_vertex(to);
        edges_.emplace_back(from, to);
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return n_;
    }

    /// O(n+m)。{成分数, 各頂点の成分番号}をトポロジカル順の番号で返す。
    std::pair<int, std::vector<int>> scc_ids() const {
        std::vector<std::vector<int>> graph(n_);
        std::vector<std::vector<int>> reverse_graph(n_);
        for (const auto& [from, to] : edges_) {
            graph[from].push_back(to);
            reverse_graph[to].push_back(from);
        }
        return detail::scc_ids_from_adjacency(graph, reverse_graph);
    }

    /// O(n+m)。成分内は頂点昇順、成分間はトポロジカル順で返す。
    std::vector<std::vector<int>> scc() const {
        const auto [group_count, ids] = scc_ids();
        return detail::scc_groups_from_ids(group_count, ids);
    }

private:
    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < n_);
    }

    int n_ = 0;
    std::vector<std::pair<int, int>> edges_;
};

/// O(n+m)。共通有向グラフの{成分数, 各頂点の成分番号}をトポロジカル順で返す。
template<directed_graph_type Graph>
std::pair<int, std::vector<int>> scc_ids(const Graph& source) {
    std::vector<std::vector<int>> graph(source.size());
    std::vector<std::vector<int>> reverse_graph(source.size());
    for (int vertex = 0; vertex < source.size(); ++vertex) {
        graph[vertex].reserve(source[vertex].size());
        for (const auto& current : source[vertex]) {
            graph[vertex].push_back(current.to);
            reverse_graph[current.to].push_back(vertex);
        }
    }
    return detail::scc_ids_from_adjacency(graph, reverse_graph);
}

/// O(n+m)。共通有向グラフを強連結成分分解し、成分をトポロジカル順で返す。
template<directed_graph_type Graph>
std::vector<std::vector<int>> scc(const Graph& graph) {
    const auto [group_count, ids] = scc_ids(graph);
    return detail::scc_groups_from_ids(group_count, ids);
}

}
