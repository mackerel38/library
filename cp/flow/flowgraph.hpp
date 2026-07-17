#pragma once
#include <bits/stdc++.h>

namespace poe {

namespace detail {

template<class Cap, class Cost>
struct flowedge {
    int from;
    int to;
    Cap cap;
    Cap flow;
    Cost cost;
};

template<class Cap>
struct flowedge<Cap, void> {
    int from;
    int to;
    Cap cap;
    Cap flow;
};

template<class Cap, class Cost>
struct residualflowedge {
    int to;
    int reverse;
    Cap cap;
    Cost cost;
};

template<class Cap>
struct residualflowedge<Cap, void> {
    int to;
    int reverse;
    Cap cap;
};

template<class Cap, class Cost>
struct flowgraphbase {
    using cap_type = Cap;
    using cost_type = Cost;
    using edge = flowedge<Cap, Cost>;
    using residual_edge = residualflowedge<Cap, Cost>;
    using stored_cost_type = std::conditional_t<std::is_void_v<Cost>, unsigned char, Cost>;

    static constexpr bool is_costed = !std::is_void_v<Cost>;

    /// O(1)。空の残余グラフを作る。
    flowgraphbase() = default;

    /// O(n)。n頂点の残余グラフを作る。
    explicit flowgraphbase(int n) : adjacency_(n) {
        assert(n >= 0);
    }

    /// 償却O(1)。容量capの辺from->toを追加し、辺番号を返す。cap>=0。
    int add_edge(int from, int to, Cap cap) requires std::is_void_v<Cost> {
        return add_edge_impl(from, to, cap, 0);
    }

    /// 償却O(1)。容量cap、単位費用costの辺from->toを追加し、辺番号を返す。
    int add_edge(int from, int to, Cap cap, const stored_cost_type& cost)
        requires (!std::is_void_v<Cost>) {
        return add_edge_impl(from, to, cap, cost);
    }

    /// O(1)。辺番号idの元容量・現在流量・存在する場合は費用を返す。
    edge edge_at(int id) const {
        assert_edge(id);
        const auto [from, position] = positions_[id];
        const residual_edge& forward = adjacency_[from][position];
        const residual_edge& reverse = adjacency_[forward.to][forward.reverse];
        if constexpr (std::is_void_v<Cost>) {
            return edge{from, forward.to, forward.cap + reverse.cap, reverse.cap};
        } else {
            return edge{
                from, forward.to, forward.cap + reverse.cap, reverse.cap, forward.cost
            };
        }
    }

    /// O(1)。ACL互換名で辺番号idの情報を返す。
    edge get_edge(int id) const {
        return edge_at(id);
    }

    /// O(m)。全辺を追加順で返す。
    std::vector<edge> edges() const {
        std::vector<edge> result;
        result.reserve(positions_.size());
        for (int id = 0; id < edge_count(); ++id) {
            result.push_back(edge_at(id));
        }
        return result;
    }

    /// O(1)。辺idを容量cap、流量flowへ変更する。0<=flow<=cap。
    void change_edge(int id, Cap cap, Cap flow) {
        assert_edge(id);
        assert(0 <= flow && flow <= cap);
        const auto [from, position] = positions_[id];
        residual_edge& forward = adjacency_[from][position];
        residual_edge& reverse = adjacency_[forward.to][forward.reverse];
        forward.cap = cap - flow;
        reverse.cap = flow;
    }

    /// O(1)。vertexから出る残余辺を返す。残余容量の変更は探索関数用。
    std::vector<residual_edge>& residual(int vertex) {
        assert_vertex(vertex);
        return adjacency_[vertex];
    }

    /// O(1)。vertexから出る残余辺を読み取り専用で返す。
    const std::vector<residual_edge>& residual(int vertex) const {
        assert_vertex(vertex);
        return adjacency_[vertex];
    }

    /// O(1)。vertexから出る残余辺を配列アクセスで返す。
    const std::vector<residual_edge>& operator[](int vertex) const {
        return residual(vertex);
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return static_cast<int>(adjacency_.size());
    }

    /// O(1)。追加した辺数を返す。
    int edge_count() const noexcept {
        return static_cast<int>(positions_.size());
    }

private:
    int add_edge_impl(int from, int to, Cap cap, const stored_cost_type& cost) {
        assert_vertex(from);
        assert_vertex(to);
        assert(cap >= 0);
        const int id = edge_count();
        int from_index = static_cast<int>(adjacency_[from].size());
        int to_index = static_cast<int>(adjacency_[to].size());
        if (from == to) {
            ++to_index;
        }
        positions_.emplace_back(from, from_index);
        if constexpr (std::is_void_v<Cost>) {
            adjacency_[from].push_back(residual_edge{to, to_index, cap});
            adjacency_[to].push_back(residual_edge{from, from_index, 0});
        } else {
            adjacency_[from].push_back(residual_edge{to, to_index, cap, cost});
            adjacency_[to].push_back(residual_edge{from, from_index, 0, -cost});
        }
        return id;
    }

    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < size());
    }

    void assert_edge(int id) const {
        assert(0 <= id && id < edge_count());
    }

    std::vector<std::pair<int, int>> positions_;
    std::vector<std::vector<residual_edge>> adjacency_;
};

}

/// コストなしフロー用残余グラフ: flowgraph<long long> graph(n)。
template<class Cap>
using flowgraph = detail::flowgraphbase<Cap, void>;

/// コスト付きフロー用残余グラフ: costflowgraph<long long, long long> graph(n)。
template<class Cap, class Cost>
using costflowgraph = detail::flowgraphbase<Cap, Cost>;

/// コストの有無を問わずフロー用残余グラフを受け取るconcept。
template<class Graph>
concept flowgraph_type = requires(Graph& graph, int vertex) {
    typename Graph::cap_type;
    typename Graph::cost_type;
    Graph::is_costed;
    { graph.size() } -> std::same_as<int>;
    { graph.edge_count() } -> std::same_as<int>;
    graph.residual(vertex);
};

/// コスト付きフロー用残余グラフだけを受け取るconcept。
template<class Graph>
concept costflowgraph_type = flowgraph_type<Graph> && Graph::is_costed;

}
