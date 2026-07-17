#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// Euler路の存在判定と復元結果: result.vertex_path, result.edge_path; 構築O(n+m)。
struct eulertrailresult {
    bool exists = false;
    std::vector<int> vertex_path;
    std::vector<int> edge_path;

    /// O(1)。Euler路が存在するならtrueを返す。
    explicit operator bool() const noexcept {
        return exists;
    }
};

/// O(n+m)。全辺を一度ずつ通るEuler路を返す。start=-1なら適切な始点を自動選択する。
template <graph_type Graph>
eulertrailresult euler_trail(const Graph& graph, int start = -1) {
    const int n = graph.size();
    const int m = graph.edge_count();
    if (start != -1) {
        assert(0 <= start && start < n);
    }
    if (m == 0) {
        if (n == 0) return {true, {}, {}};
        if (start == -1) start = 0;
        return {true, {start}, {}};
    }

    int required_start = -1;
    if constexpr (Graph::is_directed) {
        int plus = 0;
        int minus = 0;
        for (int vertex = 0; vertex < n; ++vertex) {
            const int difference = graph.outdegree(vertex) - graph.indegree(vertex);
            if (difference == 1) {
                ++plus;
                required_start = vertex;
            } else if (difference == -1) {
                ++minus;
            } else if (difference != 0) {
                return {};
            }
        }
        if (!((plus == 0 && minus == 0) || (plus == 1 && minus == 1))) {
            return {};
        }
        if (plus == 0) {
            for (int vertex = 0; vertex < n; ++vertex) {
                if (graph.outdegree(vertex) > 0) {
                    required_start = vertex;
                    break;
                }
            }
        }
    } else {
        std::vector<int> odd;
        for (int vertex = 0; vertex < n; ++vertex) {
            if (graph.degree(vertex) & 1) odd.push_back(vertex);
        }
        if (odd.size() != 0 && odd.size() != 2) {
            return {};
        }
        if (odd.size() == 2) {
            required_start = odd.front();
        } else {
            for (int vertex = 0; vertex < n; ++vertex) {
                if (graph.degree(vertex) > 0) {
                    required_start = vertex;
                    break;
                }
            }
        }
    }

    if (start == -1) {
        start = required_start;
    } else if constexpr (Graph::is_directed) {
        int plus_vertex = -1;
        for (int vertex = 0; vertex < n; ++vertex) {
            if (graph.outdegree(vertex) - graph.indegree(vertex) == 1) {
                plus_vertex = vertex;
            }
        }
        if ((plus_vertex != -1 && start != plus_vertex) || graph.outdegree(start) == 0) {
            return {};
        }
    } else {
        int odd_count = 0;
        for (int vertex = 0; vertex < n; ++vertex) odd_count += graph.degree(vertex) & 1;
        if ((odd_count == 2 && !(graph.degree(start) & 1)) || graph.degree(start) == 0) {
            return {};
        }
    }

    std::vector<int> position(n);
    std::vector<bool> used(m);
    std::vector<int> stack_vertex{start};
    std::vector<int> stack_edge;
    std::vector<int> reverse_vertices;
    std::vector<int> reverse_edges;
    while (!stack_vertex.empty()) {
        const int vertex = stack_vertex.back();
        const auto& adjacency = graph[vertex];
        while (position[vertex] < static_cast<int>(adjacency.size()) &&
               used[adjacency[position[vertex]].id]) {
            ++position[vertex];
        }
        if (position[vertex] == static_cast<int>(adjacency.size())) {
            reverse_vertices.push_back(vertex);
            stack_vertex.pop_back();
            if (!stack_edge.empty()) {
                reverse_edges.push_back(stack_edge.back());
                stack_edge.pop_back();
            }
            continue;
        }
        const auto& current = adjacency[position[vertex]++];
        if (used[current.id]) continue;
        used[current.id] = true;
        stack_vertex.push_back(current.to);
        stack_edge.push_back(current.id);
    }

    if (static_cast<int>(reverse_edges.size()) != m) {
        return {};
    }
    std::reverse(reverse_vertices.begin(), reverse_vertices.end());
    std::reverse(reverse_edges.begin(), reverse_edges.end());
    return {true, std::move(reverse_vertices), std::move(reverse_edges)};
}

}
