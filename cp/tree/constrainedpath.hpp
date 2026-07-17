#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/rootedtree.hpp"

namespace poe {

/// O(n)。端点がendpoint、内部頂点がinternalを満たす木上pathの最大頂点数を返す。
template <undirected_graph_type Graph, class EndpointPredicate, class InternalPredicate>
int longest_constrained_path(const Graph& graph,
                             EndpointPredicate endpoint,
                             InternalPredicate internal) {
    if (graph.size() == 0) return 0;
    rootedtree tree(graph, 0);
    std::vector<int> arm(graph.size());
    int answer = 0;
    for (auto iterator = tree.order().rbegin(); iterator != tree.order().rend(); ++iterator) {
        const int vertex = *iterator;
        int first = 0, second = 0;
        for (const auto& edge : graph[vertex]) {
            if (tree.parent(edge.to) != vertex) continue;
            const int value = arm[edge.to];
            if (value > first) {
                second = first;
                first = value;
            } else if (value > second) {
                second = value;
            }
        }
        if (endpoint(vertex)) {
            arm[vertex] = 1;
            answer = std::max(answer, 1 + first);
        }
        if (internal(vertex)) {
            if (first > 0) arm[vertex] = std::max(arm[vertex], first + 1);
            if (second > 0) answer = std::max(answer, first + second + 1);
        }
    }
    return answer;
}

}
