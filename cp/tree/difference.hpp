#pragma once
#include <bits/stdc++.h>
#include "tree/lca.hpp"

namespace poe {

namespace detail {

template <undirected_graph_type Graph, class T, bool Edge>
struct pathdifference {
    /// O(n log n)。全値をT{}として木上差分を作る。
    explicit pathdifference(const Graph& graph, int root = 0)
        : tree_(graph, root), edge_count_(graph.edge_count()), difference_(graph.size()) {}

    /// O(log n)。from-toパス上の全頂点または全辺へvalueを加える予約をする。
    void add(int from, int to, const T& value) {
        assert(0 <= from && from < tree_.size());
        assert(0 <= to && to < tree_.size());
        const int middle = tree_.lca(from, to);
        difference_[from] += value;
        difference_[to] += value;
        difference_[middle] -= value;
        if constexpr (Edge) {
            difference_[middle] -= value;
        } else {
            const int parent = tree_.parent(middle);
            if (parent != -1) difference_[parent] -= value;
        }
        built_ = false;
    }

    /// O(n)。予約した全加算を反映し、頂点番号順または辺番号順の値を確定する。
    void build() {
        std::vector<T> accumulated = difference_;
        if constexpr (Edge) values_.assign(edge_count_, T{});
        else values_.assign(tree_.size(), T{});
        for (int index = tree_.size() - 1; index >= 0; --index) {
            const int vertex = tree_[index];
            const int parent = tree_.parent(vertex);
            if constexpr (Edge) {
                const int edge = tree_.parent_edge(vertex);
                if (edge != -1) values_[edge] = accumulated[vertex];
            } else {
                values_[vertex] = accumulated[vertex];
            }
            if (parent != -1) accumulated[parent] += accumulated[vertex];
        }
        built_ = true;
    }

    /// O(1)。build後、頂点番号または辺番号indexの値を返す。
    const T& operator[](int index) const {
        assert(built_);
        assert(0 <= index && index < size());
        return values_[index];
    }

    /// O(1)。build後、頂点番号順または辺番号順の全値を返す。
    const std::vector<T>& values() const {
        assert(built_);
        return values_;
    }

    /// O(n)。全予約と確定値をT{}へ戻す。
    void clear() {
        std::fill(difference_.begin(), difference_.end(), T{});
        values_.clear();
        built_ = false;
    }

    /// O(1)。結果の要素数を返す。頂点版はn、辺版はn-1。
    int size() const noexcept { return Edge ? edge_count_ : tree_.size(); }

private:
    lcatree<Graph> tree_;
    int edge_count_;
    std::vector<T> difference_;
    std::vector<T> values_;
    bool built_ = false;
};

}

/// offline頂点パス加算: vertexpathdifference<Graph,long long> diff(graph); add O(log n)、build O(n)。
template <undirected_graph_type Graph, class T>
using vertexpathdifference = detail::pathdifference<Graph, T, false>;

/// offline辺パス加算: edgepathdifference<Graph,long long> diff(graph); add O(log n)、build O(n)。
template <undirected_graph_type Graph, class T>
using edgepathdifference = detail::pathdifference<Graph, T, true>;

/// 部分木・部分木外へのoffline頂点加算: subtreedifference<Graph,T> diff(graph); 各add O(1)。
template <undirected_graph_type Graph, class T>
struct subtreedifference {
    /// O(n)。全頂点をT{}としてroot根のDFS順を前処理する。
    explicit subtreedifference(const Graph& graph, int root = 0)
        : tree_(graph, root), difference_(graph.size() + 1) {}

    /// O(1)。vertexの部分木の全頂点へvalueを加える予約をする。
    void add(int vertex, const T& value) {
        assert_vertex(vertex);
        difference_[tree_.index(vertex)] += value;
        difference_[tree_.out(vertex)] -= value;
        built_ = false;
    }

    /// O(1)。vertexの部分木以外の全頂点へvalueを加える予約をする。
    void add_complement(int vertex, const T& value) {
        assert_vertex(vertex);
        difference_[0] += value;
        difference_[tree_.index(vertex)] -= value;
        difference_[tree_.out(vertex)] += value;
        difference_[tree_.size()] -= value;
        built_ = false;
    }

    /// O(n)。予約した加算を反映して頂点番号順の値を確定する。
    void build() {
        values_.assign(tree_.size(), T{});
        T current{};
        for (int index = 0; index < tree_.size(); ++index) {
            current += difference_[index];
            values_[tree_[index]] = current;
        }
        built_ = true;
    }

    /// O(1)。build後、頂点vertexの値を返す。
    const T& operator[](int vertex) const {
        assert(built_);
        assert_vertex(vertex);
        return values_[vertex];
    }

    /// O(1)。build後、頂点番号順の全値を返す。
    const std::vector<T>& values() const {
        assert(built_);
        return values_;
    }

    /// O(n)。全予約と確定値をT{}へ戻す。
    void clear() {
        std::fill(difference_.begin(), difference_.end(), T{});
        values_.clear();
        built_ = false;
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept { return tree_.size(); }

    /// O(1)。根付き木でvertexの親を返す。根では-1。
    int parent(int vertex) const { return tree_.parent(vertex); }

private:
    void assert_vertex(int vertex) const { assert(0 <= vertex && vertex < size()); }

    rootedtree<Graph> tree_;
    std::vector<T> difference_;
    std::vector<T> values_;
    bool built_ = false;
};

}
