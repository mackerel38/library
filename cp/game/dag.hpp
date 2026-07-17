#pragma once
#include <bits/stdc++.h>
#include "graph/topologicalsort.hpp"

namespace poe {

/// O(n+m)。DAG上で手を辺遷移とするimpartial gameの各頂点のGrundy数を返す。
template <directed_graph_type Graph>
std::vector<int> dag_grundy(const Graph& graph) {
    const auto topological = topological_sort(graph);
    assert(topological.is_dag());
    std::vector<int> grundy(graph.size());
    std::vector<int> seen(graph.size() + 1, -1);
    for (auto iterator = topological.order.rbegin(); iterator != topological.order.rend(); ++iterator) {
        const int vertex = *iterator;
        for (const auto& edge : graph[vertex]) {
            if (grundy[edge.to] <= graph.size()) seen[grundy[edge.to]] = vertex;
        }
        while (grundy[vertex] <= graph.size() && seen[grundy[vertex]] == vertex) ++grundy[vertex];
    }
    return grundy;
}

/// O(n+m)。DAG上の通常プレイで各頂点が先手必勝ならtrueを返す。
template <directed_graph_type Graph>
std::vector<bool> dag_winning_positions(const Graph& graph) {
    const auto grundy = dag_grundy(graph);
    std::vector<bool> result(graph.size());
    for (int vertex = 0; vertex < graph.size(); ++vertex) result[vertex] = grundy[vertex] != 0;
    return result;
}

}
