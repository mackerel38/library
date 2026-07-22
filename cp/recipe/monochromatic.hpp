#pragma once
#include <bits/stdc++.h>
#include "structure/fenwicktree.hpp"
#include "tree/heavylightdecomposition.hpp"

namespace poe {

/// recipe。二色木で色反転・重み更新・同色連結成分和を扱う。構築O(n log^2 n)、各操作O(log^2 n)。
template<class T, undirected_graph_type Graph>
struct monochromatictree {
    /// O(n log^2 n)。各頂点のweightと0/1のcolorから構築する。
    monochromatictree(
        const Graph& graph,
        const std::vector<T>& weight,
        const std::vector<int>& color,
        int root = 0
    ) : tree_(graph, root), color_(graph.size()), weight_(graph.size()),
        color_count_{fenwicktree<int>(graph.size()), fenwicktree<int>(graph.size())},
        subtree_sum_{fenwicktree<T>(graph.size()), fenwicktree<T>(graph.size())} {
        assert(weight.size() == static_cast<std::size_t>(size()));
        assert(color.size() == static_cast<std::size_t>(size()));
        for (int vertex = 0; vertex < size(); ++vertex) {
            assert(color[vertex] == 0 || color[vertex] == 1);
            color_count_[0].add(tree_[vertex], 1);
        }
        for (int vertex = 0; vertex < size(); ++vertex) add(vertex, weight[vertex]);
        for (int vertex = 0; vertex < size(); ++vertex) {
            if (color[vertex]) flip(vertex);
        }
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept { return tree_.size(); }

    /// O(1)。vertexの現在の色を返す。
    int color(int vertex) const {
        assert_vertex(vertex);
        return color_[vertex];
    }

    /// O(1)。vertexの現在の重みを返す。
    const T& operator[](int vertex) const {
        assert_vertex(vertex);
        return weight_[vertex];
    }

    /// O(log^2 n)。vertexの重みにdeltaを加える。
    void add(int vertex, const T& delta) {
        assert_vertex(vertex);
        weight_[vertex] += delta;
        const int current_color = color_[vertex];
        const int top = top_same_color(vertex);
        const int above = tree_.parent(top);
        add_path(above == -1 ? tree_.root() : above, vertex, current_color, delta);
        range_add(subtree_sum_[1 - current_color], tree_[vertex], tree_[vertex] + 1, delta);
    }

    /// O(log^2 n)。vertexの重みをvalueへ置き換える。
    void set(int vertex, const T& value) {
        assert_vertex(vertex);
        add(vertex, value - weight_[vertex]);
    }

    /// O(log^2 n)。vertexの色を反転する。
    void flip(int vertex) {
        assert_vertex(vertex);
        const int old_color = color_[vertex];
        const T old_contribution = point(subtree_sum_[old_color], tree_[vertex]);
        const int parent = tree_.parent(vertex);
        if (parent != -1) {
            const int old_top = top_same_color(vertex);
            const int above = tree_.parent(old_top);
            add_path(above == -1 ? tree_.root() : above, parent, old_color, -old_contribution);
        }

        color_count_[old_color].add(tree_[vertex], -1);
        color_[vertex] ^= 1;
        color_count_[color_[vertex]].add(tree_[vertex], 1);

        if (parent != -1) {
            const T new_contribution = point(subtree_sum_[color_[vertex]], tree_[vertex]);
            const int new_top = top_same_color(vertex);
            const int above = tree_.parent(new_top);
            add_path(above == -1 ? tree_.root() : above, parent, color_[vertex], new_contribution);
        }
    }

    /// O(log^2 n)。vertexを含む同色連結成分の重み総和を返す。
    T sum(int vertex) const {
        assert_vertex(vertex);
        const int top = top_same_color(vertex);
        return point(subtree_sum_[color_[vertex]], tree_[top]);
    }

private:
    int top_same_color(int vertex) const {
        const int opposite = 1 - color_[vertex];
        int candidate = vertex;
        for (const auto segment : tree_.path(vertex, tree_.root())) {
            assert(segment.reversed || segment.right - segment.left == 1);
            if (color_count_[opposite].sum(segment.left, segment.right) > 0) {
                const int count = color_count_[opposite].sum(segment.right);
                const int position = color_count_[opposite].lower_bound(count);
                return position + 1 < segment.right ? tree_.vertex(position + 1) : candidate;
            }
            candidate = tree_.vertex(segment.left);
        }
        return candidate;
    }

    void add_path(int from, int to, int target_color, const T& value) {
        for (const auto segment : tree_.path(from, to)) {
            range_add(subtree_sum_[target_color], segment.left, segment.right, value);
        }
    }

    void range_add(fenwicktree<T>& data, int left, int right, const T& value) {
        data.add(left, value);
        if (right < size()) data.add(right, -value);
    }

    static T point(const fenwicktree<T>& data, int index) { return data.sum(index + 1); }

    void assert_vertex(int vertex) const { assert(0 <= vertex && vertex < size()); }

    hld<Graph> tree_;
    std::vector<int> color_;
    std::vector<T> weight_;
    std::array<fenwicktree<int>, 2> color_count_;
    std::array<fenwicktree<T>, 2> subtree_sum_;
};

}
