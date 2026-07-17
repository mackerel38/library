#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 重みなし辺を表す: edge<> e{from, to, id}; 各フィールドの取得O(1)。
template<class Cost = void>
struct edge {
    int from;
    int to;
    int id;
    Cost cost;
};

/// 重みなし辺を表す特殊化。costフィールドを持たない。
template<>
struct edge<void> {
    int from;
    int to;
    int id;
};

namespace detail {

template<bool Directed, class Cost>
struct graphbase {
    using cost_type = Cost;
    using edge_type = edge<Cost>;
    using stored_cost_type = std::conditional_t<std::is_void_v<Cost>, unsigned char, Cost>;

    static constexpr bool is_directed = Directed;
    static constexpr bool is_weighted = !std::is_void_v<Cost>;

    /// O(1)。空グラフを作る。
    graphbase() = default;

    /// O(n)。n頂点の空グラフを作る。
    explicit graphbase(int n) : adjacency_(n), indegree_(n) {
        assert(n >= 0);
    }

    /// 償却O(1)。重みなし辺を追加し、その辺番号を返す。
    int add_edge(int from, int to) requires std::is_void_v<Cost> {
        assert_vertex(from);
        assert_vertex(to);
        const int id = edge_count();
        edges_.push_back(edge_type{from, to, id});
        adjacency_[from].push_back(edge_type{from, to, id});
        ++indegree_[to];
        if constexpr (!Directed) {
            adjacency_[to].push_back(edge_type{to, from, id});
            ++indegree_[from];
        }
        return id;
    }

    /// 償却O(1)。重み付き辺を追加し、その辺番号を返す。
    int add_edge(int from, int to, const stored_cost_type& cost)
        requires (!std::is_void_v<Cost>) {
        assert_vertex(from);
        assert_vertex(to);
        const int id = edge_count();
        edges_.push_back(edge_type{from, to, id, cost});
        adjacency_[from].push_back(edge_type{from, to, id, cost});
        ++indegree_[to];
        if constexpr (!Directed) {
            adjacency_[to].push_back(edge_type{to, from, id, cost});
            ++indegree_[from];
        }
        return id;
    }

    /// O(1)。頂点vertexから出る辺を返す。
    const std::vector<edge_type>& operator[](int vertex) const {
        assert_vertex(vertex);
        return adjacency_[vertex];
    }

    /// O(1)。追加時の向きで辺番号idの辺を返す。
    const edge_type& edge_at(int id) const {
        assert(0 <= id && id < edge_count());
        return edges_[id];
    }

    /// O(1)。追加した辺を返す。無向辺は一本だけ格納される。
    const std::vector<edge_type>& edges() const noexcept {
        return edges_;
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return static_cast<int>(adjacency_.size());
    }

    /// O(1)。追加した辺数を返す。無向辺も一本と数える。
    int edge_count() const noexcept {
        return static_cast<int>(edges_.size());
    }

    /// O(1)。頂点から出る辺数を返す。
    int outdegree(int vertex) const {
        assert_vertex(vertex);
        return static_cast<int>(adjacency_[vertex].size());
    }

    /// O(1)。有向グラフの頂点へ入る辺数を返す。
    int indegree(int vertex) const requires Directed {
        assert_vertex(vertex);
        return indegree_[vertex];
    }

    /// O(1)。無向グラフの次数を返す。自己辺は二回数える。
    int degree(int vertex) const requires (!Directed) {
        return outdegree(vertex);
    }

    /// O(1)。全辺一覧の再確保を減らすため、辺数の領域を予約する。
    void reserve_edges(int count) {
        assert(count >= 0);
        edges_.reserve(count);
    }

    /// O(n+m)。全有向辺を反転したグラフを返す。
    graphbase reversed() const requires Directed {
        graphbase result(size());
        result.reserve_edges(edge_count());
        for (const auto& current : edges_) {
            if constexpr (std::is_void_v<Cost>) {
                result.add_edge(current.to, current.from);
            } else {
                result.add_edge(current.to, current.from, current.cost);
            }
        }
        return result;
    }

private:
    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < size());
    }

    std::vector<std::vector<edge_type>> adjacency_;
    std::vector<edge_type> edges_;
    std::vector<int> indegree_;
};

}

/// 重み型Costの有向グラフ: directed_graph<long long> graph(n); 重みなしはCostを省略する。
template<class Cost = void>
using directed_graph = detail::graphbase<true, Cost>;

/// 重み型Costの無向グラフ: undirected_graph<long long> graph(n); 重みなしはCostを省略する。
template<class Cost = void>
using undirected_graph = detail::graphbase<false, Cost>;

/// directed_graphとundirected_graphを受け取るためのconcept。
template<class Graph>
concept graph_type = requires(const Graph& graph, int vertex) {
    typename Graph::cost_type;
    typename Graph::edge_type;
    Graph::is_directed;
    Graph::is_weighted;
    { graph.size() } -> std::same_as<int>;
    { graph.edge_count() } -> std::same_as<int>;
    { graph[vertex] } -> std::same_as<const std::vector<typename Graph::edge_type>&>;
};

/// 重み付きグラフだけを受け取るためのconcept。
template<class Graph>
concept weighted_graph_type = graph_type<Graph> && Graph::is_weighted;

/// 有向グラフだけを受け取るためのconcept。
template<class Graph>
concept directed_graph_type = graph_type<Graph> && Graph::is_directed;

/// 無向グラフだけを受け取るためのconcept。
template<class Graph>
concept undirected_graph_type = graph_type<Graph> && (!Graph::is_directed);

}
