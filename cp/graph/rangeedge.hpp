#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/shortestpath.hpp"

namespace poe {

/// range_distance_shortest_pathsへ渡す半開区間間の全方向付き遷移。
template <class Cost>
struct rangedistancetransition {
    int from_left;
    int from_right;
    int to_left;
    int to_right;
    Cost base_cost;
};

namespace detail {

template <class Cost>
struct directionalrangetree {
    int size;
    const std::vector<Cost>& position;
    const std::vector<int>& leaf_vertex;
    std::vector<int> source;
    std::vector<int> target;
    std::vector<std::tuple<int, int, Cost>>& edges;
    int& vertices;

    directionalrangetree(
        int size_,
        const std::vector<Cost>& position_,
        const std::vector<int>& leaf_vertex_,
        std::vector<std::tuple<int, int, Cost>>& edges_,
        int& vertices_
    ) : size(size_), position(position_), leaf_vertex(leaf_vertex_),
        source(4 * size_),
        target(4 * size_), edges(edges_), vertices(vertices_) {
        build(1, 0, size);
    }

    void add_transition(
        int from_left, int from_right, int to_left, int to_right, Cost base_cost) {
        const int middle = vertices++;
        const int destination = vertices++;
        add_from(1, 0, size, from_left, from_right, middle);
        edges.emplace_back(
            middle, destination,
            base_cost + position[to_left] - position[from_right - 1]);
        add_to(1, 0, size, to_left, to_right, destination);
    }

private:
    void build(int node, int left, int right) {
        if (left + 1 == right) {
            source[node] = target[node] = leaf_vertex[left];
            return;
        }
        const int middle = std::midpoint(left, right);
        build(2 * node, left, middle);
        build(2 * node + 1, middle, right);
        source[node] = vertices++;
        target[node] = vertices++;

        edges.emplace_back(
            source[2 * node], source[node],
            position[right - 1] - position[middle - 1]);
        edges.emplace_back(
            source[2 * node + 1], source[node], Cost{});

        edges.emplace_back(
            target[node], target[2 * node], Cost{});
        edges.emplace_back(
            target[node], target[2 * node + 1],
            position[middle] - position[left]);
    }

    void add_from(
        int node, int left, int right, int query_left, int query_right, int destination) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            edges.emplace_back(
                source[node], destination,
                position[query_right - 1] - position[right - 1]);
            return;
        }
        const int middle = std::midpoint(left, right);
        add_from(2 * node, left, middle, query_left, query_right, destination);
        add_from(2 * node + 1, middle, right, query_left, query_right, destination);
    }

    void add_to(
        int node, int left, int right, int query_left, int query_right, int source) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            edges.emplace_back(
                source, target[node], position[left] - position[query_left]);
            return;
        }
        const int middle = std::midpoint(left, right);
        add_to(2 * node, left, middle, query_left, query_right, source);
        add_to(2 * node + 1, middle, right, query_left, query_right, source);
    }
};

}

/// O((n+q)log n)頂点辺構築後Dijkstra。区間間のcost+座標距離遷移によるstartからの最短距離。
template <class Cost>
std::vector<Cost> range_distance_shortest_paths(
    const std::vector<Cost>& position,
    const std::vector<rangedistancetransition<Cost>>& transitions,
    int start
) {
    const int size = static_cast<int>(position.size());
    assert(size > 0 && 0 <= start && start < size);
    for (int index = 1; index < size; ++index) {
        assert(position[index - 1] <= position[index]);
    }

    int vertices = size;
    std::vector<std::tuple<int, int, Cost>> edges;
    edges.reserve(16 * size + 6 * transitions.size());
    std::vector<int> identity(size), reverse(size);
    std::iota(identity.begin(), identity.end(), 0);
    for (int index = 0; index < size; ++index) reverse[index] = size - 1 - index;
    detail::directionalrangetree<Cost> east(
        size, position, identity, edges, vertices);

    std::vector<Cost> reversed_position(size);
    for (int index = 0; index < size; ++index) {
        reversed_position[index] = -position[size - 1 - index];
    }
    detail::directionalrangetree<Cost> west(
        size, reversed_position, reverse, edges, vertices);

    for (const auto& transition : transitions) {
        assert(0 <= transition.from_left &&
               transition.from_left < transition.from_right &&
               transition.from_right <= size);
        assert(0 <= transition.to_left &&
               transition.to_left < transition.to_right &&
               transition.to_right <= size);
        assert(transition.base_cost >= Cost{});
        if (transition.from_right <= transition.to_left) {
            east.add_transition(
                transition.from_left, transition.from_right,
                transition.to_left, transition.to_right,
                transition.base_cost);
        } else {
            assert(transition.to_right <= transition.from_left);
            west.add_transition(
                size - transition.from_right, size - transition.from_left,
                size - transition.to_right, size - transition.to_left,
                transition.base_cost);
        }
    }

    directed_graph<Cost> graph(vertices);
    graph.reserve_edges(static_cast<int>(edges.size()));
    for (const auto& [from, to, cost] : edges) graph.add_edge(from, to, cost);
    const auto result = dijkstra(graph, start);
    return std::vector<Cost>(result.distance.begin(), result.distance.begin() + size);
}

}
