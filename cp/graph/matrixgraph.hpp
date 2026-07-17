#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "math/matrix.hpp"

namespace poe {

namespace detail {

template<class Cost>
struct matrix_graph_cell {
    using type = std::optional<Cost>;
};

template<>
struct matrix_graph_cell<void> {
    using type = unsigned char;
};

}

/// n*n隣接行列のグラフ: directed_matrix_graph<long long> matrix(n); 構築O(n^2)。
template<class Cost = void, bool Directed = true>
struct matrix_graph {
    using cost_type = Cost;
    using cell_type = typename detail::matrix_graph_cell<Cost>::type;
    using stored_cost_type = std::conditional_t<std::is_void_v<Cost>, unsigned char, Cost>;

    static constexpr bool is_directed = Directed;
    static constexpr bool is_weighted = !std::is_void_v<Cost>;
    static constexpr bool is_matrix_graph = true;

    /// O(1)。空グラフを作る。
    matrix_graph() = default;

    /// O(n^2)。n頂点で辺のない隣接行列グラフを作る。
    explicit matrix_graph(int n) : adjacency_(n, std::vector<cell_type>(n)) {
        assert(n >= 0);
    }

    /// O(n^2+m)。同じ向き・重み型の隣接リストグラフから変換する。
    template<graph_type Graph>
        requires (Graph::is_directed == Directed
                  && std::same_as<typename Graph::cost_type, Cost>)
    explicit matrix_graph(const Graph& source) : matrix_graph(source.size()) {
        for (const auto& current : source.edges()) {
            if constexpr (std::is_void_v<Cost>) {
                set_edge(current.from, current.to);
            } else {
                set_edge(current.from, current.to, current.cost);
            }
        }
    }

    /// O(n^2)。非zeroを辺とする重みなし二次元vectorから作る。
    template<class Value>
        requires (std::is_void_v<Cost> && std::convertible_to<Value, bool>)
    explicit matrix_graph(const std::vector<std::vector<Value>>& values)
        : matrix_graph(static_cast<int>(values.size())) {
        assert_square(values);
        for (int from = 0; from < size(); ++from) {
            const int first_to = Directed ? 0 : from;
            for (int to = first_to; to < size(); ++to) {
                if constexpr (!Directed) {
                    assert(static_cast<bool>(values[from][to])
                           == static_cast<bool>(values[to][from]));
                }
                if (static_cast<bool>(values[from][to])) set_edge(from, to);
            }
        }
    }

    /// O(n^2)。absent以外を辺重みとする二次元vectorから作る。
    explicit matrix_graph(const std::vector<std::vector<stored_cost_type>>& values,
                          const stored_cost_type& absent)
        requires (!std::is_void_v<Cost>)
        : matrix_graph(static_cast<int>(values.size())) {
        assert_square(values);
        for (int from = 0; from < size(); ++from) {
            const int first_to = Directed ? 0 : from;
            for (int to = first_to; to < size(); ++to) {
                if constexpr (!Directed) {
                    assert((values[from][to] == absent) == (values[to][from] == absent));
                    if (values[from][to] != absent) assert(values[from][to] == values[to][from]);
                }
                if (values[from][to] != absent) set_edge(from, to, values[from][to]);
            }
        }
    }

    /// O(n^2)。非zeroを辺とする重みなしmatrixから作る。
    template<class Value>
        requires (std::is_void_v<Cost> && std::convertible_to<Value, bool>)
    explicit matrix_graph(const matrix<Value>& values) : matrix_graph(values.height()) {
        assert(values.height() == values.width());
        for (int from = 0; from < size(); ++from) {
            const int first_to = Directed ? 0 : from;
            for (int to = first_to; to < size(); ++to) {
                if constexpr (!Directed) {
                    assert(static_cast<bool>(values[from][to])
                           == static_cast<bool>(values[to][from]));
                }
                if (static_cast<bool>(values[from][to])) set_edge(from, to);
            }
        }
    }

    /// O(n^2)。absent以外を辺重みとするmatrixから作る。
    explicit matrix_graph(const matrix<stored_cost_type>& values,
                          const stored_cost_type& absent)
        requires (!std::is_void_v<Cost>)
        : matrix_graph(values.height()) {
        assert(values.height() == values.width());
        for (int from = 0; from < size(); ++from) {
            const int first_to = Directed ? 0 : from;
            for (int to = first_to; to < size(); ++to) {
                if constexpr (!Directed) {
                    assert((values[from][to] == absent) == (values[to][from] == absent));
                    if (values[from][to] != absent) assert(values[from][to] == values[to][from]);
                }
                if (values[from][to] != absent) set_edge(from, to, values[from][to]);
            }
        }
    }

    /// 償却O(n)。頂点を一つ追加し、その頂点番号を返す。
    int add_vertex() {
        return add_vertices(1);
    }

    /// 償却O(n*count+count^2)。count頂点を追加し、追加区間の先頭番号を返す。
    int add_vertices(int count) {
        assert(0 <= count && count <= std::numeric_limits<int>::max() - size());
        const int first = size();
        const int next = first + count;
        for (auto& row : adjacency_) row.resize(next);
        adjacency_.resize(next, std::vector<cell_type>(next));
        return first;
    }

    /// O(1)。重みなし辺を追加または上書きする。
    void set_edge(int from, int to) requires std::is_void_v<Cost> {
        assert_vertices(from, to);
        if (!has_edge(from, to)) ++edge_count_;
        adjacency_[from][to] = 1;
        if constexpr (!Directed) adjacency_[to][from] = 1;
    }

    /// O(1)。重み付き辺を追加または上書きする。
    void set_edge(int from, int to, const stored_cost_type& cost)
        requires (!std::is_void_v<Cost>) {
        assert_vertices(from, to);
        if (!has_edge(from, to)) ++edge_count_;
        adjacency_[from][to] = cost;
        if constexpr (!Directed) adjacency_[to][from] = cost;
    }

    /// O(1)。辺があれば削除し、削除できたかを返す。
    bool remove_edge(int from, int to) {
        assert_vertices(from, to);
        if (!has_edge(from, to)) return false;
        adjacency_[from][to] = cell_type{};
        if constexpr (!Directed) adjacency_[to][from] = cell_type{};
        --edge_count_;
        return true;
    }

    /// O(1)。fromからtoへの辺があればtrueを返す。
    bool has_edge(int from, int to) const {
        assert_vertices(from, to);
        if constexpr (std::is_void_v<Cost>) {
            return adjacency_[from][to] != 0;
        } else {
            return adjacency_[from][to].has_value();
        }
    }

    /// O(1)。存在する重み付き辺の重みを返す。
    const stored_cost_type& cost(int from, int to) const requires (!std::is_void_v<Cost>) {
        assert(has_edge(from, to));
        return *adjacency_[from][to];
    }

    /// O(1)。from行を返す。重み付きcellはoptional<Cost>、重みなしcellは0/1。
    const std::vector<cell_type>& operator[](int from) const {
        assert_vertex(from);
        return adjacency_[from];
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return static_cast<int>(adjacency_.size());
    }

    /// O(1)。辺数を返す。無向辺も一本と数える。
    int edge_count() const noexcept {
        return edge_count_;
    }

    /// O(n^2)。辺なしをabsentで埋めた通常の二次元vectorへ変換する。
    std::vector<std::vector<stored_cost_type>> values(const stored_cost_type& absent) const
        requires (!std::is_void_v<Cost>) {
        std::vector result(size(), std::vector<stored_cost_type>(size(), absent));
        for (int from = 0; from < size(); ++from) {
            for (int to = 0; to < size(); ++to) {
                if (has_edge(from, to)) result[from][to] = cost(from, to);
            }
        }
        return result;
    }

    /// O(n^2)。辺なしをabsentで埋めたmatrixへ変換する。
    matrix<stored_cost_type> to_matrix(const stored_cost_type& absent) const
        requires (!std::is_void_v<Cost>) {
        return matrix<stored_cost_type>(values(absent));
    }

private:
    template<class Value>
    void assert_square(const std::vector<std::vector<Value>>& values) const {
        for (const auto& row : values) assert(static_cast<int>(row.size()) == size());
    }

    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < size());
    }

    void assert_vertices(int from, int to) const {
        assert_vertex(from);
        assert_vertex(to);
    }

    std::vector<std::vector<cell_type>> adjacency_;
    int edge_count_ = 0;
};

/// 有向隣接行列グラフ: directed_matrix_graph<long long> graph(n)。
template<class Cost = void>
using directed_matrix_graph = matrix_graph<Cost, true>;

/// 無向隣接行列グラフ: undirected_matrix_graph<long long> graph(n)。
template<class Cost = void>
using undirected_matrix_graph = matrix_graph<Cost, false>;

}
