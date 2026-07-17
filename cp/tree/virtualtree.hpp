#pragma once
#include <bits/stdc++.h>
#include "tree/lca.hpp"

namespace poe {

/// virtual tree構築結果。local indexから元頂点・親・子・圧縮辺長を取得する。
template <class Distance>
struct virtualtreeresult {
    std::vector<int> vertex;
    std::vector<int> parent;
    std::vector<std::vector<int>> children;
    std::vector<Distance> distance;
    std::vector<int> edge_length;
    std::vector<char> selected;
    std::map<int, int> position;

    /// O(1)。virtual tree頂点数を返す。
    int size() const noexcept { return static_cast<int>(vertex.size()); }

    /// O(1)。local indexに対応する元の木の頂点を返す。
    int operator[](int index) const {
        assert(0 <= index && index < size());
        return vertex[index];
    }

    /// O(log k)。元頂点のlocal indexを返す。含まれなければ-1。
    int index(int original_vertex) const {
        const auto iterator = position.find(original_vertex);
        return iterator == position.end() ? -1 : iterator->second;
    }

    /// O(1)。根のlocal indexを返す。空なら-1。
    int root() const noexcept { return vertex.empty() ? -1 : 0; }
};

/// 指定頂点と必要なLCAだけの木を作る: virtualtree builder(graph); buildはO(k log k log n)。
template <undirected_graph_type Graph>
struct virtualtree {
    using distance_type = detail::tree_distance_type_t<Graph>;

    /// O(n log n)。元の木をroot根としてLCA前処理する。
    explicit virtualtree(const Graph& graph, int root = 0) : tree_(graph, root) {}

    /// O(k log k log n)。verticesと必要なLCAからなる圧縮木を返す。重複入力は一つにする。
    virtualtreeresult<distance_type> build(std::vector<int> vertices) const {
        virtualtreeresult<distance_type> result;
        if (vertices.empty()) return result;
        for (int vertex : vertices) assert(0 <= vertex && vertex < tree_.size());

        std::sort(vertices.begin(), vertices.end());
        vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
        const std::vector<int> selected_vertices = vertices;
        std::sort(vertices.begin(), vertices.end(), [&](int left, int right) {
            return tree_.index(left) < tree_.index(right);
        });
        const int original_size = static_cast<int>(vertices.size());
        for (int index = 1; index < original_size; ++index) {
            vertices.push_back(tree_.lca(vertices[index - 1], vertices[index]));
        }
        std::sort(vertices.begin(), vertices.end(), [&](int left, int right) {
            return tree_.index(left) < tree_.index(right);
        });
        vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());

        const int k = static_cast<int>(vertices.size());
        result.vertex = vertices;
        result.parent.assign(k, -1);
        result.children.resize(k);
        result.distance.resize(k);
        result.edge_length.resize(k);
        result.selected.resize(k);
        std::vector<int> stack;
        for (int index = 0; index < k; ++index) {
            const int vertex = vertices[index];
            result.position[vertex] = index;
            result.selected[index] = std::binary_search(
                selected_vertices.begin(), selected_vertices.end(), vertex);
            while (!stack.empty() &&
                   !tree_.is_ancestor(vertices[stack.back()], vertex)) {
                stack.pop_back();
            }
            if (!stack.empty()) {
                const int parent = stack.back();
                result.parent[index] = parent;
                result.children[parent].push_back(index);
                result.distance[index] = tree_.distance(vertices[parent], vertex);
                result.edge_length[index] =
                    tree_.edge_distance(vertices[parent], vertex);
            }
            stack.push_back(index);
        }
        return result;
    }

    /// O(log n)。元の木における二頂点間の重み和を返す。
    distance_type distance(int left, int right) const {
        return tree_.distance(left, right);
    }

    /// O(log n)。元の木における二頂点間の辺数を返す。
    int edge_distance(int left, int right) const {
        return tree_.edge_distance(left, right);
    }

private:
    lcatree<Graph> tree_;
};

}
