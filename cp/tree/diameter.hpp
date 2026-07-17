#pragma once
#include <bits/stdc++.h>
#include "tree/rootedtree.hpp"

namespace poe {

/// 木の直径結果。pathは端点firstからsecondへの頂点列、edge_idsは辺番号列。
template<class Distance>
struct treediameterresult {
    Distance distance{};
    int first = -1;
    int second = -1;
    std::vector<int> path;
    std::vector<int> edge_ids;
};

/// ある頂点からの最遠頂点と距離。
template <class Distance>
struct farthestvertex {
    int vertex = -1;
    Distance distance{};
};

namespace detail {

template<undirected_graph_type Graph>
auto farthest_tree_vertex(const Graph& graph, int start, bool keep_parent) {
    using distance_type = tree_distance_type_t<Graph>;
    struct result_type {
        int vertex;
        std::vector<distance_type> distance;
        std::vector<int> parent;
        std::vector<int> parent_edge;
    };
    result_type result{start, std::vector<distance_type>(graph.size()),
                       std::vector<int>(graph.size(), -1),
                       std::vector<int>(graph.size(), -1)};
    std::vector<int> stack = {start};
    while (!stack.empty()) {
        const int vertex = stack.back();
        stack.pop_back();
        if (result.distance[result.vertex] < result.distance[vertex]) result.vertex = vertex;
        for (const auto& current : graph[vertex]) {
            if (current.id == result.parent_edge[vertex]) continue;
            result.parent[current.to] = vertex;
            result.parent_edge[current.to] = current.id;
            if constexpr (Graph::is_weighted) {
                assert(current.cost >= distance_type{});
                result.distance[current.to] = result.distance[vertex] + current.cost;
            } else {
                result.distance[current.to] = result.distance[vertex] + 1;
            }
            stack.push_back(current.to);
        }
    }
    if (!keep_parent) {
        result.parent.clear();
        result.parent_edge.clear();
    }
    return result;
}

}

/// O(n)。非負重みの無向木の直径を返す。重みなし辺は1と数える。
template<undirected_graph_type Graph>
auto tree_diameter(const Graph& graph) {
    using distance_type = detail::tree_distance_type_t<Graph>;
    treediameterresult<distance_type> result;
    if (graph.size() == 0) return result;
    rootedtree validation(graph, 0);
    static_cast<void>(validation);
    const auto first_search = detail::farthest_tree_vertex(graph, 0, false);
    const auto second_search = detail::farthest_tree_vertex(graph, first_search.vertex, true);
    result.first = first_search.vertex;
    result.second = second_search.vertex;
    result.distance = second_search.distance[result.second];
    for (int vertex = result.second; vertex != -1; vertex = second_search.parent[vertex]) {
        result.path.push_back(vertex);
        if (second_search.parent_edge[vertex] != -1) {
            result.edge_ids.push_back(second_search.parent_edge[vertex]);
        }
    }
    std::reverse(result.path.begin(), result.path.end());
    std::reverse(result.edge_ids.begin(), result.edge_ids.end());
    return result;
}

/// O(n)。各頂点からの最遠頂点を返す。同距離ではprefer(a,b)がtrueの頂点aを優先する。
template <undirected_graph_type Graph, class Prefer = std::greater<int>>
auto tree_farthest_vertices(const Graph& graph, Prefer prefer = {}) {
    using distance_type = detail::tree_distance_type_t<Graph>;
    using candidate = farthestvertex<distance_type>;
    if (graph.size() == 0) return std::vector<candidate>{};
    rootedtree tree(graph, 0);
    auto better = [&](const candidate& left, const candidate& right) {
        if (left.vertex == -1) return right;
        if (right.vertex == -1) return left;
        if (left.distance != right.distance) {
            return left.distance > right.distance ? left : right;
        }
        return std::invoke(prefer, left.vertex, right.vertex) ? left : right;
    };
    auto edge_cost = [](const auto& edge) -> distance_type {
        if constexpr (Graph::is_weighted) return edge.cost;
        else return 1;
    };
    std::vector<candidate> down(graph.size());
    for (auto iterator = tree.order().rbegin(); iterator != tree.order().rend(); ++iterator) {
        const int vertex = *iterator;
        down[vertex] = {vertex, distance_type{}};
        for (const auto& edge : graph[vertex]) {
            if (tree.parent(edge.to) != vertex) continue;
            auto current = down[edge.to];
            current.distance += edge_cost(edge);
            down[vertex] = better(down[vertex], current);
        }
    }
    struct branchcandidate {
        candidate value;
        int branch = -2;
    };
    std::vector<candidate> up(graph.size(), candidate{-1, distance_type{}});
    std::vector<candidate> result(graph.size());
    for (const int vertex : tree.order()) {
        branchcandidate first{{vertex, distance_type{}}, -1};
        branchcandidate second{{-1, distance_type{}}, -2};
        auto insert = [&](branchcandidate current) {
            if (better(current.value, first.value).vertex == current.value.vertex) {
                second = first;
                first = current;
            } else if (better(current.value, second.value).vertex == current.value.vertex) {
                second = current;
            }
        };
        if (up[vertex].vertex != -1) insert({up[vertex], tree.parent(vertex)});
        for (const auto& edge : graph[vertex]) {
            if (tree.parent(edge.to) != vertex) continue;
            auto current = down[edge.to];
            current.distance += edge_cost(edge);
            insert({current, edge.to});
        }
        result[vertex] = first.value;
        for (const auto& edge : graph[vertex]) {
            if (tree.parent(edge.to) != vertex) continue;
            up[edge.to] = (first.branch == edge.to ? second : first).value;
            up[edge.to].distance += edge_cost(edge);
        }
    }
    return result;
}

}
