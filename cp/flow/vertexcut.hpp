#pragma once
#include <bits/stdc++.h>
#include "flow/dinic.hpp"
#include "graph/graph.hpp"

namespace poe {

/// 最小重み頂点cutの結果。possible=falseなら削除可能な頂点だけでは分離できない。
template<class Cost>
struct vertexcutresult {
    bool possible;
    Cost cost;
    std::vector<int> vertices;
};

/// O(n^2(n+m))。nulloptの頂点を削除せず、sourceからsinkへの最小重み頂点cutを返す。
template<class Graph, class Cost>
vertexcutresult<Cost> minimum_vertex_cut(
    const Graph& graph,
    const std::vector<std::optional<Cost>>& costs,
    int source,
    int sink
) {
    static_assert(std::is_integral_v<Cost> && std::is_signed_v<Cost>);
    const int n = graph.size();
    assert(static_cast<int>(costs.size()) == n);
    assert(0 <= source && source < n);
    assert(0 <= sink && sink < n);
    assert(source != sink);

    Cost total = 0;
    for (const auto& cost : costs) {
        if (!cost) continue;
        assert(*cost >= 0);
        assert(*cost < std::numeric_limits<Cost>::max() - total);
        total += *cost;
    }
    const Cost infinite = total + 1;
    flowgraph<Cost> network(2 * n);
    auto in = [](int vertex) { return 2 * vertex; };
    auto out = [](int vertex) { return 2 * vertex + 1; };
    for (int vertex = 0; vertex < n; ++vertex) {
        network.add_edge(in(vertex), out(vertex), costs[vertex].value_or(infinite));
    }
    for (const auto& current : graph.edges()) {
        network.add_edge(out(current.from), in(current.to), infinite);
        if constexpr (!Graph::is_directed) {
            network.add_edge(out(current.to), in(current.from), infinite);
        }
    }

    const Cost value = dinic(network, out(source), in(sink), infinite);
    if (value == infinite) return {false, Cost{}, {}};
    const auto reachable = mincut(network, out(source));
    std::vector<int> vertices;
    for (int vertex = 0; vertex < n; ++vertex) {
        if (costs[vertex] && reachable[in(vertex)] && !reachable[out(vertex)]) {
            vertices.push_back(vertex);
        }
    }
    return {true, value, std::move(vertices)};
}

/// O(n^2(n+m))。sourceとsink以外を削除可能として最小重み頂点cutを返す。
template<class Graph, class Cost>
vertexcutresult<Cost> minimum_vertex_cut(
    const Graph& graph,
    const std::vector<Cost>& costs,
    int source,
    int sink
) {
    assert(static_cast<int>(costs.size()) == graph.size());
    std::vector<std::optional<Cost>> removable(costs.begin(), costs.end());
    removable[source] = std::nullopt;
    removable[sink] = std::nullopt;
    return minimum_vertex_cut(graph, removable, source, sink);
}

}
