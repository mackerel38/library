#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "structure/dsu.hpp"

namespace poe {

/// 最小全域森の結果。costは重み合計、edge_idsは採用した辺番号を表す。
template<class Cost>
struct minimumspanningforestresult {
    Cost cost{};
    std::vector<int> edge_ids;
    int component_count = 0;

    /// O(1)。空グラフまたは全頂点が連結ならtrueを返す。
    bool connected() const noexcept {
        return component_count <= 1;
    }
};

/// O(m log m)。重み付き無向グラフの最小全域森をKruskal法で求める。
template<weighted_graph_type Graph>
    requires (!Graph::is_directed)
auto minimum_spanning_forest(const Graph& graph) {
    using cost_type = typename Graph::cost_type;
    std::vector<int> ids(graph.edge_count());
    std::iota(ids.begin(), ids.end(), 0);
    std::ranges::sort(ids, [&](int left, int right) {
        const auto& left_edge = graph.edge_at(left);
        const auto& right_edge = graph.edge_at(right);
        if (left_edge.cost != right_edge.cost) return left_edge.cost < right_edge.cost;
        return left < right;
    });
    dsu components(graph.size());
    minimumspanningforestresult<cost_type> result;
    result.edge_ids.reserve(std::max(0, graph.size() - 1));
    for (int id : ids) {
        const auto& current = graph.edge_at(id);
        if (components.same(current.from, current.to)) continue;
        components.merge(current.from, current.to);
        result.cost += current.cost;
        result.edge_ids.push_back(id);
    }
    result.component_count = components.count();
    return result;
}

/// O(m log m)。連結な重み付き無向グラフの最小全域木を求める。非連結ならassertする。
template<weighted_graph_type Graph>
    requires (!Graph::is_directed)
auto minimum_spanning_tree(const Graph& graph) {
    auto result = minimum_spanning_forest(graph);
    assert(result.connected());
    return result;
}

}
