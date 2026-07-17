#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/lca.hpp"

namespace poe {

/// 木上の全体・部分木・二点の近い方への距離和: treedistancesum distances(graph)。
template<undirected_graph_type Graph>
    requires (!Graph::is_weighted)
struct treedistancesum {
    /// O(n log n)。重みなし木をroot根として距離和を前処理する。
    explicit treedistancesum(const Graph& graph, int root = 0)
        : tree_(graph, root), subtree_depth_sum_(graph.size()),
          ancestor_size_sum_(graph.size()), distance_sum_(graph.size()) {
        assert(graph.size() > 0);
        for (int index = 0; index < tree_.size(); ++index) {
            const int vertex = tree_[index];
            subtree_depth_sum_[vertex] = tree_.depth(vertex);
            ancestor_size_sum_[vertex] = tree_.subtree_size(vertex);
            const int parent = tree_.parent(vertex);
            if (parent != -1) ancestor_size_sum_[vertex] += ancestor_size_sum_[parent];
        }
        for (int index = tree_.size() - 1; index >= 0; --index) {
            const int vertex = tree_[index];
            const int parent = tree_.parent(vertex);
            if (parent != -1) subtree_depth_sum_[parent] += subtree_depth_sum_[vertex];
        }
        distance_sum_[tree_.root()] = subtree_depth_sum_[tree_.root()];
        for (int index = 1; index < tree_.size(); ++index) {
            const int vertex = tree_[index];
            const int parent = tree_.parent(vertex);
            distance_sum_[vertex] = distance_sum_[parent]
                + tree_.size() - 2LL * tree_.subtree_size(vertex);
        }
    }

    /// O(1)。vertexから全頂点への距離和を返す。
    long long sum(int vertex) const {
        assert_vertex(vertex);
        return distance_sum_[vertex];
    }

    /// O(log n)。sourceからsubtree_rootの固定根部分木内の全頂点への距離和を返す。
    long long subtree_sum(int source, int subtree_root) const {
        assert_vertex(source);
        assert_vertex(subtree_root);
        const long long size = tree_.subtree_size(subtree_root);
        const long long internal_depth = subtree_depth_sum_[subtree_root]
            - size * tree_.depth(subtree_root);
        if (!tree_.is_ancestor(subtree_root, source)) {
            return size * tree_.edge_distance(source, subtree_root) + internal_depth;
        }
        const long long path_sizes = ancestor_size_sum_[source]
            - ancestor_size_sum_[subtree_root];
        return size * (tree_.depth(source) - tree_.depth(subtree_root))
            + internal_depth - 2 * path_sizes;
    }

    /// O(log n)。全頂点vについてmin(distance(v,left),distance(v,right))を合計する。
    long long min_sum(int left, int right) const {
        assert_vertex(left);
        assert_vertex(right);
        if (left == right) return sum(left);
        if (tree_.depth(left) > tree_.depth(right)) std::swap(left, right);
        const int distance = tree_.edge_distance(left, right);
        const int right_region_root = tree_.kth_ancestor(right, (distance - 1) / 2);
        assert(right_region_root != -1);
        return subtree_sum(right, right_region_root)
            + sum(left) - subtree_sum(left, right_region_root);
    }

private:
    /// O(1)。頂点番号を確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < tree_.size());
    }

    lcatree<Graph> tree_;
    std::vector<long long> subtree_depth_sum_;
    std::vector<long long> ancestor_size_sum_;
    std::vector<long long> distance_sum_;
};

}
