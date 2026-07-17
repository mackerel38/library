#pragma once
#include <bits/stdc++.h>
#include "tree/rootedtree.hpp"

namespace poe {

/// O(n)。treedp(graph, root, identity, merge, add_vertex, add_edge): 各頂点の部分木DPを返す。
/// mergeは結合則とidentityを持ち、add_edge(child_value, parent_to_child_edge)で子を親へ渡す。
template<undirected_graph_type Graph, class Value, class Merge, class AddVertex, class AddEdge>
std::vector<Value> treedp(
    const Graph& graph,
    int root,
    const Value& identity,
    Merge merge,
    AddVertex add_vertex,
    AddEdge add_edge
) {
    if (graph.size() == 0) return {};
    rootedtree tree(graph, root);
    std::vector<Value> dp(graph.size(), identity);
    for (auto iterator = tree.order().rbegin(); iterator != tree.order().rend(); ++iterator) {
        const int vertex = *iterator;
        Value children = identity;
        for (const auto& current : graph[vertex]) {
            if (tree.parent(current.to) != vertex) continue;
            children = merge(children, add_edge(dp[current.to], current));
        }
        dp[vertex] = add_vertex(std::move(children), vertex);
    }
    return dp;
}

}
