#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

namespace detail {

template <std::size_t size>
inline void keep_largest(std::array<int, size>& values, int value) {
    for (int index = 0; index < static_cast<int>(size); ++index) {
        if (values[index] >= value) continue;
        std::swap(values[index], value);
    }
}

inline int largest_two_sum(std::initializer_list<int> values) {
    int first = 0;
    int second = 0;
    for (const int value : values) {
        if (first < value) {
            second = first;
            first = value;
        } else if (second < value) {
            second = value;
        }
    }
    return first + second;
}

template <undirected_graph_type Graph>
bool path_partition_width_at_most(const Graph& graph, int limit) {
    const int size = graph.size();
    if (size == 0) return true;
    std::vector<int> parent(size, -1);
    std::vector<int> order = {0};
    parent[0] = 0;
    for (int index = 0; index < size; ++index) {
        const int vertex = order[index];
        for (const auto& edge : graph[vertex]) {
            if (parent[edge.to] != -1) continue;
            parent[edge.to] = vertex;
            order.push_back(edge.to);
        }
    }
    assert(static_cast<int>(order.size()) == size);

    std::vector<int> depth(size, 1);
    std::vector<unsigned char> blocked(size);
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int vertex = *iterator;
        std::array<int, 4> extendable{};
        std::array<int, 2> closed{};
        int extendable_count = 0;
        for (const auto& edge : graph[vertex]) {
            if (parent[edge.to] != vertex) continue;
            if (blocked[edge.to]) keep_largest(closed, depth[edge.to]);
            else {
                ++extendable_count;
                keep_largest(extendable, depth[edge.to]);
            }
        }

        if (extendable_count == 0) {
            depth[vertex] = std::max(1, closed[0] + 1);
            blocked[vertex] = false;
            if (closed[0] + closed[1] + 1 > limit) return false;
            continue;
        }

        const int one_depth = std::max({
            1, extendable[0],
            extendable[1] == 0 ? 0 : extendable[1] + 1,
            closed[0] == 0 ? 0 : closed[0] + 1});
        const int one_path = largest_two_sum({
            std::max(0, extendable[0] - 1),
            extendable[1], extendable[2], closed[0], closed[1]}) + 1;

        int two_depth = std::numeric_limits<int>::max();
        int two_path = std::numeric_limits<int>::max();
        if (extendable_count >= 2) {
            two_depth = std::max({
                1, extendable[0],
                extendable[2] == 0 ? 0 : extendable[2] + 1,
                closed[0] == 0 ? 0 : closed[0] + 1});
            two_path = largest_two_sum({
                std::max(0, extendable[0] - 1),
                std::max(0, extendable[1] - 1),
                extendable[2], extendable[3], closed[0], closed[1]}) + 1;
        }

        if (one_depth <= two_depth && one_path <= limit) {
            depth[vertex] = one_depth;
            blocked[vertex] = false;
        } else {
            if (two_path > limit) return false;
            depth[vertex] = two_depth;
            blocked[vertex] = true;
        }
    }
    return true;
}

}

/// O(n log n)。各色がpathとなる頂点分割で、任意path上の最大色数の最小値を返す。
template <undirected_graph_type Graph>
int minimum_path_partition_width(const Graph& graph) {
    if (graph.size() == 0) return 0;
    int left = 0;
    int right = graph.size();
    while (right - left > 1) {
        const int middle = std::midpoint(left, right);
        if (detail::path_partition_width_at_most(graph, middle)) right = middle;
        else left = middle;
    }
    return right;
}

}
