#pragma once
#include <bits/stdc++.h>
#include "structure/fenwicktree.hpp"
#include "tree/heavylightdecomposition.hpp"

namespace poe {

/// 辺の一点更新と木のpath和を扱う: edgepathsum data(graph); 構築O(n)。
template <class T, undirected_graph_type Graph>
struct edgepathsum {
    /// O(n)。重み付き木のcostを初期値として構築する。
    explicit edgepathsum(const Graph& graph, int root = 0)
        requires Graph::is_weighted
        : decomposition_(graph, root), values_(initial_values(graph, decomposition_)) {}

    /// O(n)。辺番号順のvaluesを初期値として構築する。
    edgepathsum(const Graph& graph, const std::vector<T>& values, int root = 0)
        : decomposition_(graph, root), values_(ordered_values(graph, decomposition_, values)) {}

    /// O(log n)。辺idの値をvalueへ変更する。
    void set(int id, const T& value) {
        values_.set(decomposition_.edge_index(id), value);
    }

    /// O(log n)。辺idの値へdeltaを加える。
    void add(int id, const T& delta) {
        values_.add(decomposition_.edge_index(id), delta);
    }

    /// O(log n)。辺idの現在値を返す。
    T operator[](int id) const {
        return values_[decomposition_.edge_index(id)];
    }

    /// O(log^2 n)。fromからtoへのpathに含まれる辺の値の総和を返す。
    T sum(int from, int to) const {
        T result{};
        for (const auto& segment : decomposition_.edge_path(from, to)) {
            result += values_.sum(segment.left, segment.right);
        }
        return result;
    }

private:
    /// O(n)。重み付きgraphの辺costをHLD順へ並べる。
    static std::vector<T> initial_values(const Graph& graph, const hld<Graph>& decomposition)
        requires Graph::is_weighted {
        std::vector<T> result(graph.size());
        for (const auto& edge : graph.edges()) {
            result[decomposition.edge_index(edge.id)] = edge.cost;
        }
        return result;
    }

    /// O(n)。辺番号順の値をHLD順へ並べる。
    static std::vector<T> ordered_values(
        const Graph& graph, const hld<Graph>& decomposition, const std::vector<T>& values) {
        assert(static_cast<int>(values.size()) == graph.edge_count());
        std::vector<T> result(graph.size());
        for (int id = 0; id < graph.edge_count(); ++id) {
            result[decomposition.edge_index(id)] = values[id];
        }
        return result;
    }

    hld<Graph> decomposition_;
    fenwicktree<T> values_;
};

template <undirected_graph_type Graph>
edgepathsum(const Graph&, int = 0) -> edgepathsum<typename Graph::cost_type, Graph>;

template <undirected_graph_type Graph, class T>
edgepathsum(const Graph&, const std::vector<T>&, int = 0) -> edgepathsum<T, Graph>;

}
