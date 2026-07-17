#pragma once
#include <bits/stdc++.h>
#include "tree/rootedtree.hpp"

namespace poe {

namespace detail {

template<class Edge>
Edge reverse_tree_edge(const Edge& current) {
    if constexpr (requires { current.cost; }) {
        return Edge{current.to, current.from, current.id, current.cost};
    } else {
        return Edge{current.to, current.from, current.id};
    }
}

}

/// O(n)。木の全頂点を根としたDPを返す。mergeは結合則を満たし、identityを単位元とすること。
/// add_vertex(統合値, 頂点)、add_edge(送る頂点のDP, 送る向きの辺)を指定する。
template<undirected_graph_type Graph, class Value, class Merge, class AddVertex, class AddEdge>
std::vector<Value> rerooting(const Graph& graph, const Value& identity, Merge merge,
                             AddVertex add_vertex, AddEdge add_edge) {
    if (graph.size() == 0) return {};
    rootedtree tree(graph, 0);
    std::vector<Value> down(graph.size(), identity);
    for (auto iterator = tree.order().rbegin(); iterator != tree.order().rend(); ++iterator) {
        const int vertex = *iterator;
        Value merged = identity;
        for (const auto& current : graph[vertex]) {
            if (tree.parent(current.to) != vertex) continue;
            merged = merge(merged,
                           add_edge(down[current.to], detail::reverse_tree_edge(current)));
        }
        down[vertex] = add_vertex(merged, vertex);
    }
    std::vector<Value> upward(graph.size(), identity);
    std::vector<Value> answer(graph.size(), identity);
    for (int vertex : tree.order()) {
        const int degree = static_cast<int>(graph[vertex].size());
        std::vector<Value> contribution(degree, identity);
        for (int index = 0; index < degree; ++index) {
            const auto& current = graph[vertex][index];
            if (current.id == tree.parent_edge(vertex)) {
                contribution[index] = upward[vertex];
            } else {
                contribution[index] =
                    add_edge(down[current.to], detail::reverse_tree_edge(current));
            }
        }
        std::vector<Value> prefix(degree + 1, identity);
        std::vector<Value> suffix(degree + 1, identity);
        for (int index = 0; index < degree; ++index) {
            prefix[index + 1] = merge(prefix[index], contribution[index]);
        }
        for (int index = degree; index-- > 0;) {
            suffix[index] = merge(contribution[index], suffix[index + 1]);
        }
        answer[vertex] = add_vertex(prefix[degree], vertex);
        for (int index = 0; index < degree; ++index) {
            const auto& current = graph[vertex][index];
            if (tree.parent(current.to) != vertex) continue;
            const Value without_child = merge(prefix[index], suffix[index + 1]);
            upward[current.to] = add_edge(add_vertex(without_child, vertex), current);
        }
    }
    return answer;
}

}
