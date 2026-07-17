#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// O(n+m+K 2^K)程度。余剰辺数Kが小さい無向グラフでsource-sink単純パス数を辺数別に返す。
template<undirected_graph_type Graph>
requires (!Graph::is_weighted)
std::vector<long long> simple_path_length_counts(
    const Graph& graph,
    int source,
    int sink
) {
    const int size = graph.size();
    assert(0 <= source && source < size);
    assert(0 <= sink && sink < size);
    assert(source != sink);
    for (const auto& current : graph.edges()) assert(current.from != current.to);

    std::vector<char> active(size, true);
    std::vector<int> degree(size);
    std::queue<int> leaves;
    for (int vertex = 0; vertex < size; ++vertex) {
        degree[vertex] = graph.degree(vertex);
        if (vertex != source && vertex != sink && degree[vertex] <= 1) leaves.push(vertex);
    }
    while (!leaves.empty()) {
        const int vertex = leaves.front();
        leaves.pop();
        if (!active[vertex] || vertex == source || vertex == sink || degree[vertex] > 1) continue;
        active[vertex] = false;
        for (const auto& current : graph[vertex]) {
            if (!active[current.to]) continue;
            if (--degree[current.to] <= 1 && current.to != source && current.to != sink) {
                leaves.push(current.to);
            }
        }
    }

    std::vector<char> special(size);
    std::vector<int> compressed_id(size, -1);
    int compressed_size = 0;
    for (int vertex = 0; vertex < size; ++vertex) {
        special[vertex] = active[vertex] &&
                          (vertex == source || vertex == sink || degree[vertex] != 2);
        if (special[vertex]) compressed_id[vertex] = compressed_size++;
    }
    std::vector<std::vector<std::pair<int, int>>> compressed(compressed_size);
    std::vector<char> used(graph.edge_count());
    for (int start = 0; start < size; ++start) {
        if (!special[start]) continue;
        for (const auto& first : graph[start]) {
            if (!active[first.to] || used[first.id]) continue;
            used[first.id] = true;
            int previous_edge = first.id;
            int current = first.to;
            int length = 1;
            while (!special[current]) {
                int next_vertex = -1;
                int next_edge = -1;
                for (const auto& candidate : graph[current]) {
                    if (active[candidate.to] && candidate.id != previous_edge) {
                        next_vertex = candidate.to;
                        next_edge = candidate.id;
                        break;
                    }
                }
                assert(next_edge != -1);
                used[next_edge] = true;
                previous_edge = next_edge;
                current = next_vertex;
                ++length;
            }
            const int left = compressed_id[start];
            const int right = compressed_id[current];
            compressed[left].emplace_back(right, length);
            compressed[right].emplace_back(left, length);
        }
    }

    std::vector<long long> result(size);
    std::vector<char> visited(compressed_size);
    const int compressed_source = compressed_id[source];
    const int compressed_sink = compressed_id[sink];
    auto dfs = [&](auto&& self, int vertex, int length) -> void {
        if (vertex == compressed_sink) {
            ++result[length];
            return;
        }
        visited[vertex] = true;
        for (const auto [next, edge_length] : compressed[vertex]) {
            if (!visited[next]) self(self, next, length + edge_length);
        }
        visited[vertex] = false;
    };
    dfs(dfs, compressed_source, 0);
    return result;
}

}
