#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/shortestpath.hpp"

namespace poe {

/// O(n log n)時間・O(n)辺。辺重み(out[i]+in[j]) mod modulusの完全有向グラフで最短距離を返す。
template <std::signed_integral T>
std::vector<T> modular_complete_distances(
    const std::vector<T>& outgoing,
    const std::vector<T>& incoming,
    T modulus,
    int start = 0
) {
    const int size = static_cast<int>(outgoing.size());
    assert(size > 0 && static_cast<int>(incoming.size()) == size);
    assert(modulus > 0 && 0 <= start && start < size);
    for (const T value : outgoing) assert(0 <= value && value < modulus);
    for (const T value : incoming) assert(0 <= value && value < modulus);

    std::vector<std::pair<T, int>> order(size);
    for (int vertex = 0; vertex < size; ++vertex) order[vertex] = {incoming[vertex], vertex};
    std::sort(order.begin(), order.end());

    directed_graph<T> graph(2 * size);
    graph.reserve_edges(3 * size);
    for (int position = 0; position < size; ++position) {
        graph.add_edge(size + position, order[position].second, T{});
        const int next = (position + 1) % size;
        const T difference = next == 0
            ? modulus + order[0].first - order[position].first
            : order[next].first - order[position].first;
        graph.add_edge(size + position, size + next, difference);
    }
    for (int vertex = 0; vertex < size; ++vertex) {
        const T threshold = modulus - outgoing[vertex];
        int position = static_cast<int>(std::lower_bound(
            order.begin(), order.end(), std::pair<T, int>{threshold, -1}
        ) - order.begin());
        if (position == size) position = 0;
        graph.add_edge(
            vertex,
            size + position,
            (outgoing[vertex] + order[position].first) % modulus
        );
    }
    const auto result = dijkstra(graph, start);
    return std::vector<T>(result.distance.begin(), result.distance.begin() + size);
}

}
