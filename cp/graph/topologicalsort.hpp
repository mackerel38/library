#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// トポロジカルソート結果。dagはDAGか、uniqueは順序が一意かを表す。
struct topologicalsortresult {
    std::vector<int> order;
    bool dag = true;
    bool unique = true;

    /// O(1)。DAGならtrueを返す。
    bool is_dag() const noexcept {
        return dag;
    }

    /// O(1)。トポロジカル順序が一意ならtrueを返す。DAGでない場合はfalse。
    bool is_unique() const noexcept {
        return dag && unique;
    }
};

/// 位置区間付きトポロジカルソート結果。position[v]は頂点vを置く0-indexed位置。
struct boundedtopologicalresult {
    std::vector<int> position;

    /// O(1)。条件を満たす順序が見つかったならtrueを返す。
    bool found() const noexcept {
        return !position.empty();
    }
};

/// O(n+m)。有向グラフをトポロジカルソートし、DAG判定と一意性も返す。
template<directed_graph_type Graph>
topologicalsortresult topological_sort(const Graph& graph) {
    std::vector<int> indegree(graph.size());
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
        indegree[vertex] = graph.indegree(vertex);
    }
    std::queue<int> queue;
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
        if (indegree[vertex] == 0) queue.push(vertex);
    }
    topologicalsortresult result;
    result.order.reserve(graph.size());
    while (!queue.empty()) {
        if (queue.size() != 1) result.unique = false;
        const int vertex = queue.front();
        queue.pop();
        result.order.push_back(vertex);
        for (const auto& current : graph[vertex]) {
            if (--indegree[current.to] == 0) queue.push(current.to);
        }
    }
    result.dag = static_cast<int>(result.order.size()) == graph.size();
    if (!result.dag) result.unique = false;
    return result;
}

/// O((n+m)log n)。辞書順最小のトポロジカル順序とDAG判定を返す。
template<directed_graph_type Graph>
topologicalsortresult topological_sort_min(const Graph& graph) {
    std::vector<int> indegree(graph.size());
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
        indegree[vertex] = graph.indegree(vertex);
    }
    std::priority_queue<int, std::vector<int>, std::greater<int>> queue;
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
        if (indegree[vertex] == 0) queue.push(vertex);
    }
    topologicalsortresult result;
    result.order.reserve(graph.size());
    while (!queue.empty()) {
        if (queue.size() != 1) result.unique = false;
        const int vertex = queue.top();
        queue.pop();
        result.order.push_back(vertex);
        for (const auto& current : graph[vertex]) {
            if (--indegree[current.to] == 0) queue.push(current.to);
        }
    }
    result.dag = static_cast<int>(result.order.size()) == graph.size();
    if (!result.dag) result.unique = false;
    return result;
}

/// O((n+m)log n)。各頂点vをlower[v]以上upper[v]以下へ置くトポロジカル順序を一つ返す。
template<directed_graph_type Graph>
boundedtopologicalresult bounded_topological_sort(
    const Graph& graph,
    const std::vector<int>& lower,
    std::vector<int> upper
) {
    const int n = graph.size();
    assert(n > 0);
    assert(static_cast<int>(lower.size()) == n);
    assert(static_cast<int>(upper.size()) == n);
    for (int vertex = 0; vertex < n; ++vertex) {
        assert(0 <= lower[vertex] && lower[vertex] < n);
        assert(0 <= upper[vertex] && upper[vertex] < n);
    }

    const auto topological = topological_sort(graph);
    if (!topological.is_dag()) return {};
    for (auto iterator = topological.order.rbegin(); iterator != topological.order.rend(); ++iterator) {
        const int vertex = *iterator;
        for (const auto& current : graph[vertex]) {
            upper[vertex] = std::min(upper[vertex], upper[current.to] - 1);
        }
    }
    for (int vertex = 0; vertex < n; ++vertex) {
        if (upper[vertex] < lower[vertex]) return {};
    }

    std::vector<int> indegree(n);
    std::vector<std::vector<int>> released_at(n);
    for (int vertex = 0; vertex < n; ++vertex) {
        indegree[vertex] = graph.indegree(vertex);
        if (indegree[vertex] == 0) released_at[lower[vertex]].push_back(vertex);
    }

    using candidate = std::pair<int, int>;
    std::priority_queue<candidate, std::vector<candidate>, std::greater<candidate>> available;
    boundedtopologicalresult result{std::vector<int>(n, -1)};
    for (int position = 0; position < n; ++position) {
        for (const int vertex : released_at[position]) {
            available.emplace(upper[vertex], vertex);
        }
        if (available.empty() || available.top().first < position) return {};
        const int vertex = available.top().second;
        available.pop();
        result.position[vertex] = position;
        for (const auto& current : graph[vertex]) {
            if (--indegree[current.to] != 0) continue;
            const int release = std::max(position + 1, lower[current.to]);
            if (release >= n) return {};
            released_at[release].push_back(current.to);
        }
    }
    return result;
}

}
