#pragma once
#include <bits/stdc++.h>
#include "graph/connectivity.hpp"
#include "graph/graph.hpp"

namespace poe {

/// LowLinkの結果。articulation_pointsは関節点、bridge_idsは橋の辺番号を表す。
struct lowlinkresult {
    std::vector<int> order;
    std::vector<int> low;
    std::vector<int> articulation_points;
    std::vector<int> bridge_ids;
    std::vector<char> bridge;

    /// O(1)。辺番号idが橋ならtrueを返す。
    bool is_bridge(int id) const {
        assert(0 <= id && id < static_cast<int>(bridge.size()));
        return bridge[id];
    }
};

/// O(n+m)。無向グラフの関節点と橋を反復DFSで求める。多重辺と自己辺も使える。
template<undirected_graph_type Graph>
lowlinkresult lowlink(const Graph& graph) {
    lowlinkresult result;
    result.order.assign(graph.size(), -1);
    result.low.resize(graph.size());
    result.bridge.assign(graph.edge_count(), false);
    std::vector<int> parent(graph.size(), -1);
    std::vector<int> parent_edge(graph.size(), -1);
    std::vector<int> child_count(graph.size());
    std::vector<int> next_edge(graph.size());
    std::vector<char> articulation(graph.size());
    int timer = 0;
    for (int root = 0; root < graph.size(); ++root) {
        if (result.order[root] != -1) continue;
        result.order[root] = result.low[root] = timer++;
        std::vector<int> stack = {root};
        while (!stack.empty()) {
            const int vertex = stack.back();
            if (next_edge[vertex] < static_cast<int>(graph[vertex].size())) {
                const auto& current = graph[vertex][next_edge[vertex]++];
                if (current.id == parent_edge[vertex]) continue;
                if (result.order[current.to] == -1) {
                    parent[current.to] = vertex;
                    parent_edge[current.to] = current.id;
                    ++child_count[vertex];
                    result.order[current.to] = result.low[current.to] = timer++;
                    stack.push_back(current.to);
                } else {
                    result.low[vertex] = std::min(result.low[vertex], result.order[current.to]);
                }
                continue;
            }
            stack.pop_back();
            if (parent[vertex] == -1) {
                if (child_count[vertex] >= 2) articulation[vertex] = true;
                continue;
            }
            const int previous = parent[vertex];
            result.low[previous] = std::min(result.low[previous], result.low[vertex]);
            if (result.order[previous] < result.low[vertex]) {
                result.bridge[parent_edge[vertex]] = true;
            }
            if (parent[previous] != -1 && result.order[previous] <= result.low[vertex]) {
                articulation[previous] = true;
            }
        }
    }
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
        if (articulation[vertex]) result.articulation_points.push_back(vertex);
    }
    for (int id = 0; id < graph.edge_count(); ++id) {
        if (result.bridge[id]) result.bridge_ids.push_back(id);
    }
    return result;
}

/// O(n+m)。橋を除いて無向グラフを二辺連結成分分解する。
template<undirected_graph_type Graph>
componentresult two_edge_connected_components(const Graph& graph) {
    const lowlinkresult links = lowlink(graph);
    componentresult result;
    result.id.assign(graph.size(), -1);
    for (int start = 0; start < graph.size(); ++start) {
        if (result.id[start] != -1) continue;
        result.groups.emplace_back();
        result.id[start] = result.count;
        std::vector<int> stack = {start};
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            result.groups.back().push_back(vertex);
            for (const auto& current : graph[vertex]) {
                if (links.is_bridge(current.id) || result.id[current.to] != -1) continue;
                result.id[current.to] = result.count;
                stack.push_back(current.to);
            }
        }
        std::ranges::sort(result.groups.back());
        ++result.count;
    }
    return result;
}

}
