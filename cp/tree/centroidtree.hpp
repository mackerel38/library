#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "tree/rootedtree.hpp"

namespace poe {

/// 元頂点からcentroid ancestorへの距離。branchはcentroidから最初に進む元頂点。
template <class Distance>
struct centroidpathentry {
    int centroid;
    Distance distance;
    int branch;
};

/// 木を重心で再帰分割する: centroidtree decomposition(graph); 構築O(n log n)。
template <undirected_graph_type Graph>
struct centroidtree {
    using distance_type = detail::tree_distance_type_t<Graph>;

    /// O(n log n)。元の木のcentroid decompositionと全頂点からancestor重心への距離を構築する。
    explicit centroidtree(const Graph& graph)
        : parent_(graph.size(), -1), depth_(graph.size()), children_(graph.size()),
          path_(graph.size()), removed_(graph.size()), component_parent_(graph.size()),
          component_size_(graph.size()) {
        if (graph.size() == 0) return;
        assert(graph.edge_count() == graph.size() - 1);
        root_ = decompose(graph, 0, -1, 0);
        assert(static_cast<int>(order_.size()) == graph.size());
    }

    /// O(1)。centroid treeの根を返す。空木では-1。
    int root() const noexcept { return root_; }

    /// O(1)。vertexのcentroid tree上の親を返す。根では-1。
    int parent(int vertex) const {
        assert_vertex(vertex);
        return parent_[vertex];
    }

    /// O(1)。vertexのcentroid tree上の深さを返す。
    int depth(int vertex) const {
        assert_vertex(vertex);
        return depth_[vertex];
    }

    /// O(1)。vertexのcentroid tree上の子を返す。
    const std::vector<int>& children(int vertex) const {
        assert_vertex(vertex);
        return children_[vertex];
    }

    /// O(1)。元頂点vertexからcentroid ancestorへの情報を根側から順に返す。
    const std::vector<centroidpathentry<distance_type>>& path(int vertex) const {
        assert_vertex(vertex);
        return path_[vertex];
    }

    /// O(1)。構築順index番目のcentroidを返す。
    int operator[](int index) const {
        assert(0 <= index && index < size());
        return order_[index];
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept { return static_cast<int>(parent_.size()); }

    /// O(1)。centroidの構築順を返す。親は必ず子より前にある。
    const std::vector<int>& order() const noexcept { return order_; }

private:
    int decompose(const Graph& graph, int start, int centroid_parent, int depth) {
        std::vector<int> component;
        std::vector<std::pair<int, int>> stack = {{start, -1}};
        while (!stack.empty()) {
            const auto [vertex, parent] = stack.back();
            stack.pop_back();
            component_parent_[vertex] = parent;
            component.push_back(vertex);
            for (const auto& edge : graph[vertex]) {
                if (removed_[edge.to] || edge.to == parent) continue;
                stack.emplace_back(edge.to, vertex);
            }
        }
        for (auto iterator = component.rbegin(); iterator != component.rend(); ++iterator) {
            const int vertex = *iterator;
            component_size_[vertex] = 1;
            for (const auto& edge : graph[vertex]) {
                if (!removed_[edge.to] && component_parent_[edge.to] == vertex) {
                    component_size_[vertex] += component_size_[edge.to];
                }
            }
        }

        const int total = static_cast<int>(component.size());
        int centroid = start;
        int best_largest = total;
        for (int vertex : component) {
            int largest = total - component_size_[vertex];
            for (const auto& edge : graph[vertex]) {
                if (!removed_[edge.to] && component_parent_[edge.to] == vertex) {
                    largest = std::max(largest, component_size_[edge.to]);
                }
            }
            if (largest < best_largest) {
                best_largest = largest;
                centroid = vertex;
            }
        }
        assert(2 * best_largest <= total);

        parent_[centroid] = centroid_parent;
        depth_[centroid] = depth;
        if (centroid_parent != -1) children_[centroid_parent].push_back(centroid);
        order_.push_back(centroid);
        path_[centroid].push_back({centroid, distance_type{}, -1});
        for (const auto& first_edge : graph[centroid]) {
            if (removed_[first_edge.to]) continue;
            const distance_type first_distance = [&] {
                if constexpr (Graph::is_weighted) return first_edge.cost;
                else return distance_type{1};
            }();
            struct state {
                int vertex;
                int parent;
                distance_type distance;
            };
            std::vector<state> distance_stack = {
                {first_edge.to, centroid, first_distance}};
            while (!distance_stack.empty()) {
                const state current = distance_stack.back();
                distance_stack.pop_back();
                path_[current.vertex].push_back(
                    {centroid, current.distance, first_edge.to});
                for (const auto& edge : graph[current.vertex]) {
                    if (removed_[edge.to] || edge.to == current.parent ||
                        edge.to == centroid) {
                        continue;
                    }
                    const distance_type edge_distance = [&] {
                        if constexpr (Graph::is_weighted) return edge.cost;
                        else return distance_type{1};
                    }();
                    distance_stack.push_back(
                        {edge.to, current.vertex, current.distance + edge_distance});
                }
            }
        }

        removed_[centroid] = true;
        for (const auto& edge : graph[centroid]) {
            if (!removed_[edge.to]) decompose(graph, edge.to, centroid, depth + 1);
        }
        return centroid;
    }

    void assert_vertex(int vertex) const { assert(0 <= vertex && vertex < size()); }

    int root_ = -1;
    std::vector<int> parent_;
    std::vector<int> depth_;
    std::vector<std::vector<int>> children_;
    std::vector<std::vector<centroidpathentry<distance_type>>> path_;
    std::vector<char> removed_;
    std::vector<int> component_parent_;
    std::vector<int> component_size_;
    std::vector<int> order_;
};

}
