#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// 閉路検出結果。vertices[i]からedge_ids[i]を通りvertices[(i+1)%k]へ進む。
struct cycleresult {
    std::vector<int> vertices;
    std::vector<int> edge_ids;

    /// O(1)。閉路が見つかったならtrueを返す。
    bool found() const noexcept {
        return !edge_ids.empty();
    }
};

/// 重み付き最小cycle。vertices[i]からedge_ids[i]を通りvertices[(i+1)%k]へ進む。
template<class Cost>
struct minimumcycleresult {
    Cost cost{};
    std::vector<int> vertices;
    std::vector<int> edge_ids;

    /// O(1)。cycleが見つかったならtrueを返す。
    bool found() const noexcept {
        return !edge_ids.empty();
    }
};

namespace detail {

template<graph_type Graph>
cycleresult directed_cycle(const Graph& graph) {
    std::vector<char> state(graph.size());
    std::vector<int> parent(graph.size(), -1);
    std::vector<int> parent_edge(graph.size(), -1);
    std::vector<int> next_edge(graph.size());
    for (int start = 0; start < graph.size(); ++start) {
        if (state[start] != 0) continue;
        state[start] = 1;
        std::vector<int> stack = {start};
        while (!stack.empty()) {
            const int vertex = stack.back();
            if (next_edge[vertex] == static_cast<int>(graph[vertex].size())) {
                state[vertex] = 2;
                stack.pop_back();
                continue;
            }
            const auto& current = graph[vertex][next_edge[vertex]++];
            if (state[current.to] == 0) {
                state[current.to] = 1;
                parent[current.to] = vertex;
                parent_edge[current.to] = current.id;
                stack.push_back(current.to);
                continue;
            }
            if (state[current.to] != 1) continue;
            cycleresult result;
            for (int node = vertex; node != current.to; node = parent[node]) {
                result.vertices.push_back(node);
                result.edge_ids.push_back(parent_edge[node]);
            }
            result.vertices.push_back(current.to);
            std::reverse(result.vertices.begin(), result.vertices.end());
            std::reverse(result.edge_ids.begin(), result.edge_ids.end());
            result.edge_ids.push_back(current.id);
            return result;
        }
    }
    return {};
}

template<graph_type Graph>
cycleresult undirected_cycle(const Graph& graph) {
    std::vector<char> state(graph.size());
    std::vector<int> parent(graph.size(), -1);
    std::vector<int> parent_edge(graph.size(), -1);
    std::vector<int> depth(graph.size());
    std::vector<int> next_edge(graph.size());
    for (int start = 0; start < graph.size(); ++start) {
        if (state[start] != 0) continue;
        state[start] = 1;
        std::vector<int> stack = {start};
        while (!stack.empty()) {
            const int vertex = stack.back();
            if (next_edge[vertex] == static_cast<int>(graph[vertex].size())) {
                state[vertex] = 2;
                stack.pop_back();
                continue;
            }
            const auto& current = graph[vertex][next_edge[vertex]++];
            if (current.id == parent_edge[vertex]) continue;
            if (state[current.to] == 0) {
                state[current.to] = 1;
                parent[current.to] = vertex;
                parent_edge[current.to] = current.id;
                depth[current.to] = depth[vertex] + 1;
                stack.push_back(current.to);
                continue;
            }
            if (state[current.to] != 1 || depth[current.to] > depth[vertex]) continue;
            cycleresult result;
            for (int node = vertex; node != current.to; node = parent[node]) {
                result.vertices.push_back(node);
                result.edge_ids.push_back(parent_edge[node]);
            }
            result.vertices.push_back(current.to);
            std::reverse(result.vertices.begin(), result.vertices.end());
            std::reverse(result.edge_ids.begin(), result.edge_ids.end());
            result.edge_ids.push_back(current.id);
            return result;
        }
    }
    return {};
}

}

/// O(n+m)。有向・無向グラフから閉路を一つ返す。存在しなければ空の結果を返す。
template<graph_type Graph>
cycleresult find_cycle(const Graph& graph) {
    if constexpr (Graph::is_directed) {
        return detail::directed_cycle(graph);
    } else {
        return detail::undirected_cycle(graph);
    }
}

/// O(n^2+m)。非負重み無向グラフでrootを通る最小単純cycleを返す。forbidden_edgeは使用しない。
template<undirected_graph_type Graph>
requires Graph::is_weighted
minimumcycleresult<typename Graph::cost_type> minimum_cycle_through_vertex(
    const Graph& graph,
    int root,
    int forbidden_edge = -1
) {
    using Cost = typename Graph::cost_type;
    assert(0 <= root && root < graph.size());
    assert(-1 <= forbidden_edge && forbidden_edge < graph.edge_count());
    const Cost infinity = std::numeric_limits<Cost>::max() / 4;
    std::vector<Cost> distance(graph.size(), infinity);
    std::vector<int> label(graph.size(), -1);
    std::vector<int> parent(graph.size(), -1);
    std::vector<int> parent_edge(graph.size(), -1);
    std::vector<char> used(graph.size());
    distance[root] = Cost{};
    label[root] = root;
    for (int iteration = 0; iteration < graph.size(); ++iteration) {
        int vertex = -1;
        for (int candidate = 0; candidate < graph.size(); ++candidate) {
            if (
                !used[candidate] && distance[candidate] != infinity
                && (vertex == -1 || distance[candidate] < distance[vertex])
            ) {
                vertex = candidate;
            }
        }
        if (vertex == -1) break;
        used[vertex] = true;
        for (const auto& current : graph[vertex]) {
            assert(!(current.cost < Cost{}));
            if (
                current.id == forbidden_edge
                || distance[vertex] > infinity - current.cost
            ) {
                continue;
            }
            const Cost candidate = distance[vertex] + current.cost;
            if (candidate < distance[current.to]) {
                distance[current.to] = candidate;
                parent[current.to] = vertex;
                parent_edge[current.to] = current.id;
                label[current.to] = vertex == root ? current.to : label[vertex];
            }
        }
    }

    Cost best = infinity;
    int left_end = -1, right_end = -1, closing_edge = -1;
    for (const auto& current : graph.edges()) {
        if (
            current.id == forbidden_edge
            || distance[current.from] == infinity || distance[current.to] == infinity
            || label[current.from] == label[current.to]
            || parent_edge[current.from] == current.id
            || parent_edge[current.to] == current.id
            || distance[current.from] > infinity - current.cost
            || distance[current.from] + current.cost > infinity - distance[current.to]
        ) {
            continue;
        }
        const Cost candidate =
            distance[current.from] + current.cost + distance[current.to];
        if (candidate < best) {
            best = candidate;
            left_end = current.from;
            right_end = current.to;
            closing_edge = current.id;
        }
    }
    if (left_end == -1) return {};

    auto root_path = [&](int vertex) {
        std::vector<int> vertices;
        std::vector<int> edges;
        while (vertex != root) {
            vertices.push_back(vertex);
            edges.push_back(parent_edge[vertex]);
            vertex = parent[vertex];
        }
        vertices.push_back(root);
        std::reverse(vertices.begin(), vertices.end());
        std::reverse(edges.begin(), edges.end());
        return std::pair{std::move(vertices), std::move(edges)};
    };
    auto [left_vertices, left_edges] = root_path(left_end);
    auto [right_vertices, right_edges] = root_path(right_end);
    minimumcycleresult<Cost> result;
    result.cost = best;
    result.vertices = std::move(left_vertices);
    result.edge_ids = std::move(left_edges);
    result.edge_ids.push_back(closing_edge);
    for (auto iterator = right_vertices.rbegin();
         iterator != right_vertices.rend() - 1; ++iterator) {
        result.vertices.push_back(*iterator);
    }
    for (auto iterator = right_edges.rbegin(); iterator != right_edges.rend(); ++iterator) {
        result.edge_ids.push_back(*iterator);
    }
    assert(result.vertices.size() == result.edge_ids.size());
    return result;
}

/// O(2^n n^2)時間・O(2^n n)領域。n<=20の多重無向グラフの頂点単純cycle数を返す。
template <class T>
T count_simple_cycles(const std::vector<std::vector<int>>& multiplicity) {
    const int size = static_cast<int>(multiplicity.size());
    assert(size <= 20);
    for (const auto& row : multiplicity) {
        assert(static_cast<int>(row.size()) == size);
    }
    T answer{};
    for (int first = 0; first < size; ++first) {
        assert(multiplicity[first][first] == 0);
        for (int second = first + 1; second < size; ++second) {
            assert(multiplicity[first][second] == multiplicity[second][first]);
            assert(multiplicity[first][second] >= 0);
            answer += T{multiplicity[first][second]} *
                T{multiplicity[first][second] - 1} / T{2};
        }
    }

    T oriented{};
    for (int start = 0; start < size; ++start) {
        const int local_size = size - start;
        const int masks = 1 << local_size;
        std::vector<T> dp(static_cast<std::size_t>(masks) * local_size);
        auto at = [&](int mask, int vertex) -> T& {
            return dp[static_cast<std::size_t>(mask) * local_size + vertex];
        };
        at(1, 0) = T{1};
        for (int mask = 1; mask < masks; mask += 2) {
            const int vertices = std::popcount(static_cast<unsigned>(mask));
            for (int last = 0; last < local_size; ++last) {
                const T current = at(mask, last);
                if (current == T{}) continue;
                if (vertices >= 3) {
                    oriented += current * T{multiplicity[start + last][start]};
                }
                for (int next = 1; next < local_size; ++next) {
                    if (mask >> next & 1) continue;
                    const int edges = multiplicity[start + last][start + next];
                    if (edges == 0) continue;
                    at(mask | (1 << next), next) += current * T{edges};
                }
            }
        }
    }
    return answer + oriented / T{2};
}

}
