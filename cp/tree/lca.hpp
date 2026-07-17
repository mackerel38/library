#pragma once
#include <bits/stdc++.h>
#include "tree/rootedtree.hpp"

namespace poe {

/// 二分累乗でLCAと木上移動を扱う: lcatree tree(graph, root); 構築O(n log n)、取得O(log n)。
template<undirected_graph_type Graph>
struct lcatree {
    using distance_type = detail::tree_distance_type_t<Graph>;

    /// O(n log n)。無向木をroot根として前処理する。
    explicit lcatree(const Graph& graph, int root = 0) : tree_(graph, root) {
        int levels = 1;
        while ((1LL << levels) <= std::max(1, tree_.size())) ++levels;
        ancestor_.assign(levels, std::vector<int>(tree_.size()));
        for (int vertex = 0; vertex < tree_.size(); ++vertex) {
            ancestor_[0][vertex] = tree_.parent(vertex) == -1 ? vertex : tree_.parent(vertex);
        }
        for (int level = 1; level < levels; ++level) {
            for (int vertex = 0; vertex < tree_.size(); ++vertex) {
                ancestor_[level][vertex] = ancestor_[level - 1][ancestor_[level - 1][vertex]];
            }
        }
    }

    /// O(log n)。頂点vertexから上へstep辺移動した頂点を返す。存在しなければ-1。
    int kth_ancestor(int vertex, int step) const {
        assert_vertex(vertex);
        assert(step >= 0);
        if (step > tree_.depth(vertex)) return -1;
        for (int level = 0; step > 0; ++level, step >>= 1) {
            if (step & 1) vertex = ancestor_[level][vertex];
        }
        return vertex;
    }

    /// O(log n)。二頂点の最小共通祖先を返す。
    int lca(int left, int right) const {
        assert_vertex(left);
        assert_vertex(right);
        if (tree_.depth(left) < tree_.depth(right)) std::swap(left, right);
        left = kth_ancestor(left, tree_.depth(left) - tree_.depth(right));
        if (left == right) return left;
        for (int level = static_cast<int>(ancestor_.size()) - 1; level >= 0; --level) {
            if (ancestor_[level][left] == ancestor_[level][right]) continue;
            left = ancestor_[level][left];
            right = ancestor_[level][right];
        }
        return tree_.parent(left);
    }

    /// O(log n)。fromからtoへの単純路をstep辺進んだ頂点を返す。路を越えるなら-1。
    int jump(int from, int to, int step) const {
        assert_vertex(from);
        assert_vertex(to);
        assert(step >= 0);
        const int middle = lca(from, to);
        const int upward = tree_.depth(from) - tree_.depth(middle);
        const int length = upward + tree_.depth(to) - tree_.depth(middle);
        if (step > length) return -1;
        if (step <= upward) return kth_ancestor(from, step);
        return kth_ancestor(to, length - step);
    }

    /// O(log n)。二頂点間の重み和を返す。重みなし辺は1と数える。
    distance_type distance(int left, int right) const {
        const int middle = lca(left, right);
        return tree_.distance(left) + tree_.distance(right) -
               tree_.distance(middle) - tree_.distance(middle);
    }

    /// O(log n)。二頂点間の辺数を返す。
    int edge_distance(int left, int right) const {
        const int middle = lca(left, right);
        return tree_.depth(left) + tree_.depth(right) - 2 * tree_.depth(middle);
    }

    /// O(log n)。vertexがfromからtoへの単純路上にあればtrueを返す。
    bool on_path(int vertex, int from, int to) const {
        assert_vertex(vertex);
        return edge_distance(from, vertex) + edge_distance(vertex, to) == edge_distance(from, to);
    }

    /// O(log n)。vertexからtowardへ向かう最初の辺を切ったとき、vertex側の頂点数を返す。
    /// vertex == towardなら木全体の頂点数を返す。
    int side_size(int vertex, int toward) const {
        assert_vertex(vertex);
        assert_vertex(toward);
        if (vertex == toward) return size();
        const int neighbor = jump(vertex, toward, 1);
        if (tree_.parent(neighbor) == vertex) return size() - tree_.subtree_size(neighbor);
        return tree_.subtree_size(vertex);
    }

    /// O(1)。頂点vertexの親を返す。根では-1。
    int parent(int vertex) const {
        return tree_.parent(vertex);
    }

    /// O(1)。親へ向かう辺番号を返す。根では-1。
    int parent_edge(int vertex) const {
        return tree_.parent_edge(vertex);
    }

    /// O(1)。根から頂点vertexまでの辺数を返す。
    int depth(int vertex) const {
        return tree_.depth(vertex);
    }

    /// O(1)。頂点vertexの部分木頂点数を返す。
    int subtree_size(int vertex) const {
        return tree_.subtree_size(vertex);
    }

    /// O(1)。ancestorがvertexの祖先ならtrueを返す。
    bool is_ancestor(int ancestor, int vertex) const {
        return tree_.is_ancestor(ancestor, vertex);
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return tree_.size();
    }

    /// O(1)。根を返す。空木では-1。
    int root() const noexcept {
        return tree_.root();
    }

    /// O(1)。頂点vertexのDFS行きがけ順の位置を返す。
    int index(int vertex) const {
        return tree_.index(vertex);
    }

    /// O(1)。頂点vertexの部分木区間の右端を返す。
    int out(int vertex) const {
        return tree_.out(vertex);
    }

    /// O(1)。DFS行きがけ順index番目の頂点を返す。
    int operator[](int index) const {
        return tree_[index];
    }

    /// O(1)。根からvertexまでの重み和を返す。重みなし辺は1。
    distance_type root_distance(int vertex) const {
        return tree_.distance(vertex);
    }

private:
    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < size());
    }

    rootedtree<Graph> tree_;
    std::vector<std::vector<int>> ancestor_;
};

}
