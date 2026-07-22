#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// O(n+m)。連結無向graphで各頂点の訪問回数をtarget parityにする長さ4n未満のwalkを返す。
template<undirected_graph_type Graph>
std::vector<int> vertex_parity_walk(
    const Graph& graph,
    const std::vector<int>& target,
    int root = 0
) {
    const int n = graph.size();
    assert(static_cast<int>(target.size()) == n);
    assert(n == 0 || (0 <= root && root < n));
    for (const int parity : target) assert(parity == 0 || parity == 1);
    if (n == 0) return {};

    struct frame {
        int vertex;
        int next_edge = 0;
    };
    std::vector<int> parent(n, -2), parity(n), walk;
    std::vector<frame> stack;
    walk.reserve(4 * n);
    stack.reserve(n);
    parent[root] = -1;
    walk.push_back(root);
    parity[root] ^= 1;
    stack.push_back({root, 0});

    while (!stack.empty()) {
        auto& current = stack.back();
        const auto& edges = graph[current.vertex];
        while (current.next_edge < static_cast<int>(edges.size())
               && parent[edges[current.next_edge].to] != -2) {
            ++current.next_edge;
        }
        if (current.next_edge < static_cast<int>(edges.size())) {
            const int child = edges[current.next_edge++].to;
            parent[child] = current.vertex;
            walk.push_back(child);
            parity[child] ^= 1;
            stack.push_back({child, 0});
            continue;
        }

        const int vertex = current.vertex;
        if (vertex != root && parity[vertex] != target[vertex]) {
            walk.push_back(parent[vertex]);
            parity[parent[vertex]] ^= 1;
            walk.push_back(vertex);
            parity[vertex] ^= 1;
        }
        stack.pop_back();
        if (vertex != root) {
            walk.push_back(parent[vertex]);
            parity[parent[vertex]] ^= 1;
        }
    }
    assert(std::ranges::none_of(parent, [](int value) { return value == -2; }));
    if (parity[root] != target[root]) walk.erase(walk.begin());
    assert(static_cast<int>(walk.size()) < 4 * n);
    return walk;
}

}  // namespace poe
