#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

namespace detail {

template<class Graph, bool Weighted = Graph::is_weighted>
struct tree_distance_type {
    using type = long long;
};

template<class Graph>
struct tree_distance_type<Graph, true> {
    using type = typename Graph::cost_type;
};

template<class Graph>
using tree_distance_type_t = typename tree_distance_type<Graph>::type;

}

/// 無向木をroot根の根付き木として前処理する: rootedtree tree(graph, root); 構築O(n)。
template<undirected_graph_type Graph>
struct rootedtree {
    using distance_type = detail::tree_distance_type_t<Graph>;

    /// O(n)。無向木をroot根で前処理する。空でないグラフは連結な木であること。
    explicit rootedtree(const Graph& graph, int root = 0)
        : root_(root), parent_(graph.size(), -2), parent_edge_(graph.size(), -1),
          depth_(graph.size()), distance_(graph.size()), in_(graph.size()),
          out_(graph.size()), subtree_size_(graph.size(), 1) {
        if (graph.size() == 0) {
            assert(root == 0);
            root_ = -1;
            return;
        }
        assert(0 <= root && root < graph.size());
        assert(graph.edge_count() == graph.size() - 1);
        parent_[root] = -1;
        std::vector<int> stack = {root};
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            in_[vertex] = static_cast<int>(order_.size());
            order_.push_back(vertex);
            for (auto iterator = graph[vertex].rbegin(); iterator != graph[vertex].rend();
                 ++iterator) {
                const auto& current = *iterator;
                if (current.id == parent_edge_[vertex]) continue;
                assert(parent_[current.to] == -2);
                parent_[current.to] = vertex;
                parent_edge_[current.to] = current.id;
                depth_[current.to] = depth_[vertex] + 1;
                if constexpr (Graph::is_weighted) {
                    distance_[current.to] = distance_[vertex] + current.cost;
                } else {
                    distance_[current.to] = distance_[vertex] + 1;
                }
                stack.push_back(current.to);
            }
        }
        assert(static_cast<int>(order_.size()) == graph.size());
        for (auto iterator = order_.rbegin(); iterator != order_.rend(); ++iterator) {
            const int vertex = *iterator;
            if (parent_[vertex] != -1) {
                subtree_size_[parent_[vertex]] += subtree_size_[vertex];
            }
            out_[vertex] = in_[vertex] + subtree_size_[vertex];
        }
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return static_cast<int>(parent_.size());
    }

    /// O(1)。根を返す。空木では-1を返す。
    int root() const noexcept {
        return root_;
    }

    /// O(1)。頂点vertexの親を返す。根では-1を返す。
    int parent(int vertex) const {
        assert_vertex(vertex);
        return parent_[vertex];
    }

    /// O(1)。親へ向かう辺番号を返す。根では-1を返す。
    int parent_edge(int vertex) const {
        assert_vertex(vertex);
        return parent_edge_[vertex];
    }

    /// O(1)。根から頂点vertexまでの辺数を返す。
    int depth(int vertex) const {
        assert_vertex(vertex);
        return depth_[vertex];
    }

    /// O(1)。根から頂点vertexまでの重み和を返す。重みなし辺は1と数える。
    distance_type distance(int vertex) const {
        assert_vertex(vertex);
        return distance_[vertex];
    }

    /// O(1)。頂点vertexの部分木頂点数を返す。
    int subtree_size(int vertex) const {
        assert_vertex(vertex);
        return subtree_size_[vertex];
    }

    /// O(1)。頂点vertexのDFS行きがけ順の位置を返す。
    int index(int vertex) const {
        assert_vertex(vertex);
        return in_[vertex];
    }

    /// O(1)。頂点vertexの部分木を表す半開区間[index(vertex), out(vertex))の右端を返す。
    int out(int vertex) const {
        assert_vertex(vertex);
        return out_[vertex];
    }

    /// O(1)。行きがけ順index番目の頂点を返す。
    int operator[](int index) const {
        assert(0 <= index && index < size());
        return order_[index];
    }

    /// O(1)。DFS行きがけ順の全頂点を返す。
    const std::vector<int>& order() const noexcept {
        return order_;
    }

    /// O(1)。ancestorがvertexの祖先ならtrueを返す。
    bool is_ancestor(int ancestor, int vertex) const {
        assert_vertex(ancestor);
        assert_vertex(vertex);
        return in_[ancestor] <= in_[vertex] && in_[vertex] < out_[ancestor];
    }

private:
    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < size());
    }

    int root_ = -1;
    std::vector<int> parent_;
    std::vector<int> parent_edge_;
    std::vector<int> depth_;
    std::vector<distance_type> distance_;
    std::vector<int> in_;
    std::vector<int> out_;
    std::vector<int> subtree_size_;
    std::vector<int> order_;
};

}
