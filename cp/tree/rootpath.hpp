#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// O(n+各callback)。stateを追加・rollbackし、根から各頂点へのpath状態を全頂点で評価する。
template<undirected_graph_type Graph, class State, class Add, class Get>
auto root_path_rollback(
    const Graph& graph,
    int root,
    State& state,
    Add add,
    Get get
) {
    using result_type = std::remove_cvref_t<std::invoke_result_t<Get, State&, int>>;
    const int n = graph.size();
    assert(n > 0);
    assert(0 <= root && root < n);
    assert(graph.edge_count() == n - 1);
    std::vector<result_type> result(n);
    struct event {
        int vertex;
        int parent_edge;
        int snapshot;
        bool leaving;
    };
    std::vector<event> stack{{root, -1, 0, false}};
    std::vector<char> seen(n);
    int visited = 0;
    while (!stack.empty()) {
        const event current = stack.back();
        stack.pop_back();
        if (current.leaving) {
            state.rollback(current.snapshot);
            continue;
        }
        assert(!seen[current.vertex]);
        seen[current.vertex] = true;
        ++visited;
        const int snapshot = state.snapshot();
        std::invoke(add, state, current.vertex);
        result[current.vertex] = std::invoke(get, state, current.vertex);
        stack.push_back({current.vertex, current.parent_edge, snapshot, true});
        for (auto iterator = graph[current.vertex].rbegin();
             iterator != graph[current.vertex].rend(); ++iterator) {
            if (iterator->id == current.parent_edge) continue;
            stack.push_back({iterator->to, iterator->id, 0, false});
        }
    }
    assert(visited == n);
    return result;
}

}
