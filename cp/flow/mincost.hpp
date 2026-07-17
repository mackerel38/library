#pragma once
#include <bits/stdc++.h>
#include "flow/flowgraph.hpp"

namespace poe {

/// O(F(m+n log n))。費用の折れ点をlimitまで返す。各辺の初期費用は非負。
template<costflowgraph_type Graph>
std::vector<std::pair<typename Graph::cap_type, typename Graph::cost_type>> mincostslope(
    Graph& graph,
    int source,
    int sink,
    typename Graph::cap_type limit
) {
    using Cap = typename Graph::cap_type;
    using Cost = typename Graph::cost_type;
    assert(0 <= source && source < graph.size());
    assert(0 <= sink && sink < graph.size());
    assert(source != sink);
    assert(limit >= 0);
    for (const auto& current : graph.edges()) {
        assert(current.cost >= 0);
        assert(current.flow == 0);
    }
    const int n = graph.size();
    const Cost infinity = std::numeric_limits<Cost>::max();
    std::vector<Cost> potential(n);
    std::vector<Cost> distance(n);
    std::vector<int> previous_vertex(n);
    std::vector<int> previous_edge(n);
    std::vector<std::pair<Cap, Cost>> result = {{0, 0}};
    Cap current_flow = 0;
    Cost current_cost = 0;
    std::optional<Cost> previous_cost_per_flow;
    while (current_flow < limit) {
        std::fill(distance.begin(), distance.end(), infinity);
        distance[source] = 0;
        using state = std::pair<Cost, int>;
        std::priority_queue<state, std::vector<state>, std::greater<state>> queue;
        queue.emplace(0, source);
        while (!queue.empty()) {
            const auto [current_distance, vertex] = queue.top();
            queue.pop();
            if (distance[vertex] != current_distance) {
                continue;
            }
            const auto& edges = graph.residual(vertex);
            for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
                const auto& current = edges[i];
                if (current.cap == 0) {
                    continue;
                }
                const Cost next_distance = current_distance + current.cost +
                                           potential[vertex] - potential[current.to];
                if (next_distance < distance[current.to]) {
                    distance[current.to] = next_distance;
                    previous_vertex[current.to] = vertex;
                    previous_edge[current.to] = i;
                    queue.emplace(next_distance, current.to);
                }
            }
        }
        if (distance[sink] == infinity) {
            break;
        }
        for (int vertex = 0; vertex < n; ++vertex) {
            if (distance[vertex] != infinity) {
                potential[vertex] += distance[vertex];
            }
        }
        Cap add = limit - current_flow;
        for (int vertex = sink; vertex != source; vertex = previous_vertex[vertex]) {
            add = std::min(
                add,
                graph.residual(previous_vertex[vertex])[previous_edge[vertex]].cap
            );
        }
        for (int vertex = sink; vertex != source; vertex = previous_vertex[vertex]) {
            auto& current = graph.residual(previous_vertex[vertex])[previous_edge[vertex]];
            current.cap -= add;
            graph.residual(vertex)[current.reverse].cap += add;
        }
        const Cost cost_per_flow = potential[sink] - potential[source];
        current_flow += add;
        current_cost += static_cast<Cost>(add) * cost_per_flow;
        if (previous_cost_per_flow && *previous_cost_per_flow == cost_per_flow) {
            result.pop_back();
        }
        result.emplace_back(current_flow, current_cost);
        previous_cost_per_flow = cost_per_flow;
    }
    return result;
}

/// O(F(m+n log n))。費用の折れ点を最大流まで返す。各辺の初期費用は非負。
template<costflowgraph_type Graph>
std::vector<std::pair<typename Graph::cap_type, typename Graph::cost_type>> mincostslope(
    Graph& graph,
    int source,
    int sink
) {
    return mincostslope(
        graph, source, sink, std::numeric_limits<typename Graph::cap_type>::max()
    );
}

/// O(F(m+n log n))。sourceからsinkへlimitまで流した流量と最小費用を返す。
template<costflowgraph_type Graph>
std::pair<typename Graph::cap_type, typename Graph::cost_type> mincost(
    Graph& graph,
    int source,
    int sink,
    typename Graph::cap_type limit
) {
    return mincostslope(graph, source, sink, limit).back();
}

/// O(F(m+n log n))。sourceからsinkへ最大限流した流量と最小費用を返す。
template<costflowgraph_type Graph>
std::pair<typename Graph::cap_type, typename Graph::cost_type> mincost(
    Graph& graph,
    int source,
    int sink
) {
    return mincostslope(graph, source, sink).back();
}

/// mincost_transportへ渡す有向輸送辺。capacityとcostは非負。
template<class Cap, class Cost>
struct costtransportedge {
    int from;
    int to;
    Cap capacity;
    Cost cost;
};

/// mincost_transportの結果。feasible()なら全需要を満たしcostが最小費用。
template<class Cap, class Cost>
struct mincosttransportresult {
    Cap flow;
    Cap demand;
    Cost cost;

    /// O(1)。全需要を満たせたならtrueを返す。
    bool feasible() const noexcept { return flow == demand; }
};

/// O(F(m+n log n))。頂点ごとの供給上限から必須需要へ輸送する最小費用を返す。
template<class Cap, class Cost>
mincosttransportresult<Cap, Cost> mincost_transport(
    const std::vector<Cap>& supply,
    const std::vector<Cap>& demand,
    const std::vector<costtransportedge<Cap, Cost>>& edges
) {
    assert(supply.size() == demand.size());
    const int size = static_cast<int>(supply.size());
    const int source = size;
    const int sink = size + 1;
    costflowgraph<Cap, Cost> graph(size + 2);
    Cap total_demand{};
    for (int vertex = 0; vertex < size; ++vertex) {
        assert(supply[vertex] >= 0 && demand[vertex] >= 0);
        if (supply[vertex] > 0) graph.add_edge(source, vertex, supply[vertex], Cost{});
        if (demand[vertex] > 0) graph.add_edge(vertex, sink, demand[vertex], Cost{});
        total_demand += demand[vertex];
    }
    for (const auto& edge : edges) {
        assert(0 <= edge.from && edge.from < size);
        assert(0 <= edge.to && edge.to < size);
        assert(edge.capacity >= 0 && edge.cost >= 0);
        graph.add_edge(edge.from, edge.to, edge.capacity, edge.cost);
    }
    const auto [flow, cost] = mincost(graph, source, sink, total_demand);
    return {flow, total_demand, cost};
}

}
