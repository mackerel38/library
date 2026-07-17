#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// 連結成分分解の結果: result[vertex]で成分番号を取得する。
struct componentresult {
    int count = 0;
    std::vector<int> id;
    std::vector<std::vector<int>> groups;
    std::vector<int> edge_count;

    /// O(1)。頂点vertexの成分番号を返す。
    int operator[](int vertex) const {
        assert(0 <= vertex && vertex < static_cast<int>(id.size()));
        return id[vertex];
    }

    /// O(1)。二頂点が同じ成分に属するか返す。
    bool same(int left, int right) const {
        return (*this)[left] == (*this)[right];
    }

    /// O(1)。componentが木ならtrueを返す。孤立頂点も木として扱う。
    bool is_tree(int component) const {
        assert(0 <= component && component < count);
        return edge_count[component] + 1 == static_cast<int>(groups[component].size());
    }
};

/// O(n+m)。無向グラフを連結成分分解する。成分内と成分間は頂点番号順になる。
template<undirected_graph_type Graph>
componentresult connected_components(const Graph& graph) {
    componentresult result;
    result.id.assign(graph.size(), -1);
    for (int start = 0; start < graph.size(); ++start) {
        if (result.id[start] != -1) continue;
        result.groups.emplace_back();
        std::vector<int> stack = {start};
        result.id[start] = result.count;
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            result.groups.back().push_back(vertex);
            for (const auto& current : graph[vertex]) {
                if (result.id[current.to] != -1) continue;
                result.id[current.to] = result.count;
                stack.push_back(current.to);
            }
        }
        std::ranges::sort(result.groups.back());
        ++result.count;
    }
    result.edge_count.assign(result.count, 0);
    for (const auto& edge : graph.edges()) ++result.edge_count[result.id[edge.from]];
    return result;
}

/// 二部グラフ判定の結果。colorは各頂点の0/1彩色、失敗時のconflict_edgeは同色を結ぶ辺番号。
struct bipartiteresult {
    std::vector<int> color;
    int conflict_edge = -1;

    /// O(1)。二部グラフならtrueを返す。
    bool is_bipartite() const noexcept {
        return conflict_edge == -1;
    }

    /// O(1)。頂点vertexの色0/1を返す。
    int operator[](int vertex) const {
        assert(0 <= vertex && vertex < static_cast<int>(color.size()));
        return color[vertex];
    }
};

/// O(n+m)。無向グラフを二部彩色し、二部でない場合は衝突辺を返す。
template<undirected_graph_type Graph>
bipartiteresult bipartite_coloring(const Graph& graph) {
    bipartiteresult result;
    result.color.assign(graph.size(), -1);
    for (int start = 0; start < graph.size(); ++start) {
        if (result.color[start] != -1) continue;
        result.color[start] = 0;
        std::queue<int> queue;
        queue.push(start);
        while (!queue.empty()) {
            const int vertex = queue.front();
            queue.pop();
            for (const auto& current : graph[vertex]) {
                if (result.color[current.to] == -1) {
                    result.color[current.to] = result.color[vertex] ^ 1;
                    queue.push(current.to);
                } else if (result.color[current.to] == result.color[vertex]) {
                    result.conflict_edge = current.id;
                    return result;
                }
            }
        }
    }
    return result;
}

/// O(n+m)。無向グラフが二部グラフならtrueを返す。
template<undirected_graph_type Graph>
bool is_bipartite(const Graph& graph) {
    return bipartite_coloring(graph).is_bipartite();
}

}
