#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// 頂点有効化に伴う到達情報。reached_at[v]は初めて到達するstep、count[i]はstep i後の到達数。
struct incrementalreachabilityresult {
    std::vector<int> reached_at;
    std::vector<int> count;
};

/// O(n+m)。order順に頂点を有効化した各時点の、sourcesからの有効頂点内到達集合を返す。
template <directed_graph_type Graph>
incrementalreachabilityresult incremental_reachability(
    const Graph& graph,
    const std::vector<int>& order,
    const std::vector<int>& sources
) {
    const int n = graph.size();
    assert(static_cast<int>(order.size()) == n);
    std::vector<bool> is_source(n), active(n), pending(n), seen_order(n);
    for (int source : sources) {
        assert(0 <= source && source < n);
        is_source[source] = true;
    }
    incrementalreachabilityresult result{std::vector<int>(n, -1), std::vector<int>(n)};
    int reached_count = 0;
    std::vector<int> stack;
    for (int step = 0; step < n; ++step) {
        const int vertex = order[step];
        assert(0 <= vertex && vertex < n && !seen_order[vertex]);
        seen_order[vertex] = true;
        active[vertex] = true;
        if (is_source[vertex] || pending[vertex]) stack.push_back(vertex);
        while (!stack.empty()) {
            const int current = stack.back();
            stack.pop_back();
            if (result.reached_at[current] != -1) continue;
            result.reached_at[current] = step;
            ++reached_count;
            for (const auto& edge : graph[current]) {
                if (result.reached_at[edge.to] != -1) continue;
                if (active[edge.to]) stack.push_back(edge.to);
                else pending[edge.to] = true;
            }
        }
        result.count[step] = reached_count;
    }
    return result;
}

/// O(n+m)。頂点0,1,...の順に有効化した各時点のsourceからの到達情報を返す。
template <directed_graph_type Graph>
incrementalreachabilityresult incremental_reachability(const Graph& graph, int source) {
    std::vector<int> order(graph.size());
    std::iota(order.begin(), order.end(), 0);
    return incremental_reachability(graph, order, std::vector<int>{source});
}

}
