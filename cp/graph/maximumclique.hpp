#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// 最大clique・最大独立集合の結果。verticesは条件を満たす最大頂点集合。
struct maximumvertexsetresult {
    std::vector<int> vertices;

    /// O(1)。選ばれた頂点数を返す。
    int size() const noexcept {
        return static_cast<int>(vertices.size());
    }
};

namespace detail {

/// 彩色上界付きbranch-and-boundで最大cliqueを探索する。頂点数は63以下。
struct maximumcliquesolver {
    explicit maximumcliquesolver(std::vector<std::uint64_t> adjacent_)
        : adjacent(std::move(adjacent_)) {}

    maximumvertexsetresult solve() {
        const int n = static_cast<int>(adjacent.size());
        if (n == 0) return {};
        expand((std::uint64_t{1} << n) - 1);
        return {best};
    }

private:
    /// O(k^2)。候補をgreedy coloringし、各prefixのclique数上界を付ける。
    void color_sort(std::uint64_t candidates, std::vector<int>& order,
                    std::vector<int>& colors) const {
        int color = 0;
        std::uint64_t remaining = candidates;
        while (remaining != 0) {
            ++color;
            std::uint64_t available = remaining;
            while (available != 0) {
                const int vertex = std::countr_zero(available);
                const std::uint64_t bit = std::uint64_t{1} << vertex;
                order.push_back(vertex);
                colors.push_back(color);
                remaining ^= bit;
                available &= ~adjacent[vertex];
                available &= remaining;
            }
        }
    }

    /// worst O(2^n)。彩色数で枝刈りしながらcliqueを伸ばす。
    void expand(std::uint64_t candidates) {
        if (candidates == 0) {
            if (current.size() > best.size()) best = current;
            return;
        }
        std::vector<int> order;
        std::vector<int> colors;
        order.reserve(std::popcount(candidates));
        colors.reserve(order.capacity());
        color_sort(candidates, order, colors);

        for (int index = static_cast<int>(order.size()) - 1; index >= 0; --index) {
            if (current.size() + static_cast<std::size_t>(colors[index]) <= best.size()) return;
            const int vertex = order[index];
            const std::uint64_t bit = std::uint64_t{1} << vertex;
            current.push_back(vertex);
            const std::uint64_t next = candidates & adjacent[vertex];
            if (next == 0) {
                if (current.size() > best.size()) best = current;
            } else {
                expand(next);
            }
            current.pop_back();
            candidates ^= bit;
        }
    }

    std::vector<std::uint64_t> adjacent;
    std::vector<int> current;
    std::vector<int> best;
};

template<undirected_graph_type Graph>
std::vector<std::uint64_t> adjacency_masks(const Graph& graph) {
    assert(graph.size() <= 63);
    std::vector<std::uint64_t> adjacent(graph.size());
    for (const auto& edge : graph.edges()) {
        if (edge.from == edge.to) continue;
        adjacent[edge.from] |= std::uint64_t{1} << edge.to;
        adjacent[edge.to] |= std::uint64_t{1} << edge.from;
    }
    return adjacent;
}

}

/// worst O(2^n)。63頂点以下の無向グラフの最大cliqueを彩色枝刈りで求める。
template<undirected_graph_type Graph>
maximumvertexsetresult maximum_clique(const Graph& graph) {
    return detail::maximumcliquesolver(detail::adjacency_masks(graph)).solve();
}

/// worst O(2^n)。63頂点以下の無向グラフの最大独立集合を補グラフの最大cliqueとして求める。
template<undirected_graph_type Graph>
maximumvertexsetresult maximum_independent_set(const Graph& graph) {
    auto adjacent = detail::adjacency_masks(graph);
    const int n = graph.size();
    const std::uint64_t all = n == 0 ? 0 : (std::uint64_t{1} << n) - 1;
    for (int vertex = 0; vertex < n; ++vertex) {
        adjacent[vertex] = all & ~adjacent[vertex] & ~(std::uint64_t{1} << vertex);
    }
    return detail::maximumcliquesolver(std::move(adjacent)).solve();
}

}
