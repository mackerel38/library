#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// 通常辺と「同じgroup内の任意頂点へ1手」を持つBFS: bfs_with_hyperedges(n, groups, s, expand); O(n+m+incidence)。
/// expand(v, visit)内で通常辺の行先ごとにvisit(to)を呼ぶ。
template <class Expand>
std::vector<int> bfs_with_hyperedges(int vertex_count,
                                     const std::vector<std::vector<int>>& groups,
                                     int start, Expand expand) {
    assert(vertex_count >= 0);
    assert(0 <= start && start < vertex_count);
    std::vector<std::vector<int>> memberships(vertex_count);
    for (int group = 0; group < static_cast<int>(groups.size()); ++group) {
        for (int vertex : groups[group]) {
            assert(0 <= vertex && vertex < vertex_count);
            memberships[vertex].push_back(group);
        }
    }

    std::vector<int> distance(vertex_count, -1);
    std::vector<bool> used_group(groups.size());
    std::queue<int> queue;
    distance[start] = 0;
    queue.push(start);
    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop();
        auto visit = [&](int to) {
            assert(0 <= to && to < vertex_count);
            if (distance[to] != -1) return;
            distance[to] = distance[vertex] + 1;
            queue.push(to);
        };
        std::invoke(expand, vertex, visit);
        for (int group : memberships[vertex]) {
            if (used_group[group]) continue;
            used_group[group] = true;
            for (int to : groups[group]) visit(to);
        }
    }
    return distance;
}

/// 通常辺をgraphから読むhyperedge BFS: bfs_with_hyperedges(graph, groups, start); O(n+m+incidence)。
template <graph_type Graph>
std::vector<int> bfs_with_hyperedges(const Graph& graph,
                                     const std::vector<std::vector<int>>& groups,
                                     int start) {
    return bfs_with_hyperedges(graph.size(), groups, start,
        [&](int vertex, auto visit) {
            for (const auto& edge : graph[vertex]) visit(edge.to);
        });
}

}
