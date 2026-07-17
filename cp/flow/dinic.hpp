#pragma once
#include <bits/stdc++.h>
#include "flow/flowgraph.hpp"

namespace poe {

/// O(n^2m)。graph上でsourceからsinkへlimitまで最大流を追加し、その量を返す。
template<flowgraph_type Graph>
typename Graph::cap_type dinic(
    Graph& graph,
    int source,
    int sink,
    typename Graph::cap_type limit
) {
    using Cap = typename Graph::cap_type;
    assert(0 <= source && source < graph.size());
    assert(0 <= sink && sink < graph.size());
    assert(source != sink);
    assert(limit >= 0);
    Cap result = 0;
    std::vector<int> level(graph.size());
    std::vector<int> iterator(graph.size());
    while (result < limit) {
        std::fill(level.begin(), level.end(), -1);
        std::queue<int> queue;
        level[source] = 0;
        queue.push(source);
        while (!queue.empty()) {
            const int vertex = queue.front();
            queue.pop();
            for (const auto& current : graph.residual(vertex)) {
                if (current.cap > 0 && level[current.to] == -1) {
                    level[current.to] = level[vertex] + 1;
                    queue.push(current.to);
                }
            }
        }
        if (level[sink] == -1) {
            break;
        }
        std::fill(iterator.begin(), iterator.end(), 0);
        auto dfs = [&](auto&& self, int vertex, Cap up) -> Cap {
            if (vertex == sink) {
                return up;
            }
            auto& edges = graph.residual(vertex);
            for (int& i = iterator[vertex]; i < static_cast<int>(edges.size()); ++i) {
                auto& current = edges[i];
                if (current.cap <= 0 || level[vertex] >= level[current.to]) {
                    continue;
                }
                const Cap pushed = self(self, current.to, std::min(up, current.cap));
                if (pushed == 0) {
                    continue;
                }
                current.cap -= pushed;
                graph.residual(current.to)[current.reverse].cap += pushed;
                return pushed;
            }
            return 0;
        };
        while (result < limit) {
            const Cap pushed = dfs(dfs, source, limit - result);
            if (pushed == 0) {
                break;
            }
            result += pushed;
        }
    }
    return result;
}

/// O(n^2m)。graph上でsourceからsinkへ流せる最大流を追加し、その量を返す。
template<flowgraph_type Graph>
typename Graph::cap_type dinic(Graph& graph, int source, int sink) {
    return dinic(graph, source, sink, std::numeric_limits<typename Graph::cap_type>::max());
}

/// O(n+m)。残余グラフでsourceから到達できる頂点を返す。最大流後は最小カット。
template<flowgraph_type Graph>
std::vector<bool> mincut(const Graph& graph, int source) {
    assert(0 <= source && source < graph.size());
    std::vector<bool> visited(graph.size());
    std::queue<int> queue;
    visited[source] = true;
    queue.push(source);
    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop();
        for (const auto& current : graph.residual(vertex)) {
            if (current.cap > 0 && !visited[current.to]) {
                visited[current.to] = true;
                queue.push(current.to);
            }
        }
    }
    return visited;
}

}
