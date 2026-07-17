#pragma once
#include <bits/stdc++.h>
#include "structure/sparsetable.hpp"
#include "tree/eulertour.hpp"

namespace poe {

namespace detail {

inline std::pair<int, int> minimum_depth_vertex(
    const std::pair<int, int>& left,
    const std::pair<int, int>& right
) {
    return std::min(left, right);
}

}

/// 動的頂点集合の直径。空ならleft=right=-1、singletonならleft=right。
struct vertexsetdiameterresult {
    int left = -1;
    int right = -1;
    int distance = 0;

    /// O(1)。集合が空ならtrue。
    bool empty() const noexcept { return left == -1; }
};

/// 頂点の有効無効を更新し、有効頂点間の最大辺距離を管理する。構築O(n log n)、更新O(log n)。
template<undirected_graph_type Graph>
struct vertexsetdiameter {
    /// O(n log n)。全頂点をactiveにして構築する。
    explicit vertexsetdiameter(const Graph& graph)
        : vertexsetdiameter(graph, std::vector<bool>(graph.size(), true)) {}

    /// O(n log n)。active[v]で初期状態を指定する。
    vertexsetdiameter(const Graph& graph, const std::vector<bool>& active)
        : tour_(graph), rmq_(make_rmq_values()), active_(active) {
        const int n = graph.size();
        assert(static_cast<int>(active.size()) == n);
        while (leaf_count_ < std::max(1, n)) leaf_count_ <<= 1;
        data_.assign(leaf_count_ << 1, {});
        for (int vertex = 0; vertex < n; ++vertex) {
            if (active_[vertex]) data_[leaf_count_ + vertex] = singleton(vertex);
        }
        for (int node = leaf_count_ - 1; node > 0; --node) pull(node);
    }

    /// O(1)。元の木の頂点数を返す。
    int size() const noexcept { return static_cast<int>(active_.size()); }

    /// O(1)。vertexが有効ならtrue。
    bool active(int vertex) const {
        assert_vertex(vertex);
        return active_[vertex];
    }

    /// O(log n)。vertexの有効無効をvalueへ変更する。
    void set(int vertex, bool value) {
        assert_vertex(vertex);
        if (active_[vertex] == value) return;
        active_[vertex] = value;
        int node = leaf_count_ + vertex;
        data_[node] = value ? singleton(vertex) : vertexsetdiameterresult{};
        while ((node >>= 1) > 0) pull(node);
    }

    /// O(log n)。vertexの有効無効を反転する。
    void flip(int vertex) { set(vertex, !active(vertex)); }

    /// O(1)。有効頂点集合の直径端点と距離を返す。
    const vertexsetdiameterresult& get() const noexcept { return data_[1]; }

    /// O(1)。二頂点間の辺数を返す。
    int distance(int left, int right) const {
        assert_vertex(left);
        assert_vertex(right);
        const int first_left = tour_.first(left);
        const int first_right = tour_.first(right);
        const int begin = std::min(first_left, first_right);
        const int end = std::max(first_left, first_right) + 1;
        const int lca_depth = rmq_.prod(begin, end).first;
        return tour_.depth(first_left) + tour_.depth(first_right) - 2 * lca_depth;
    }

private:
    std::vector<std::pair<int, int>> make_rmq_values() const {
        std::vector<std::pair<int, int>> values(tour_.size());
        for (int i = 0; i < tour_.size(); ++i) values[i] = {tour_.depth(i), tour_[i]};
        return values;
    }

    static vertexsetdiameterresult singleton(int vertex) {
        return {vertex, vertex, 0};
    }

    vertexsetdiameterresult merge(
        const vertexsetdiameterresult& left,
        const vertexsetdiameterresult& right
    ) const {
        if (left.empty()) return right;
        if (right.empty()) return left;
        const std::array<int, 4> vertices{left.left, left.right, right.left, right.right};
        vertexsetdiameterresult result = left.distance >= right.distance ? left : right;
        for (int i = 0; i < 4; ++i) {
            for (int j = i + 1; j < 4; ++j) {
                const int candidate = distance(vertices[i], vertices[j]);
                if (candidate > result.distance) {
                    result = {vertices[i], vertices[j], candidate};
                }
            }
        }
        return result;
    }

    void pull(int node) { data_[node] = merge(data_[node << 1], data_[node << 1 | 1]); }

    void assert_vertex(int vertex) const { assert(0 <= vertex && vertex < size()); }

    eulertour<Graph> tour_;
    sparsetable<std::pair<int, int>, detail::minimum_depth_vertex> rmq_;
    std::vector<bool> active_;
    int leaf_count_ = 1;
    std::vector<vertexsetdiameterresult> data_;
};

}
