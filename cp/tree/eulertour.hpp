#pragma once
#include <bits/stdc++.h>
#include "tree/rootedtree.hpp"

namespace poe {

/// 無向木を根から巡回し、頂点へ入る時と子から戻る時の列を作る: eulertour tour(graph, root); 構築O(n)。
template<undirected_graph_type Graph>
struct eulertour {
    /// O(n)。隣接リストの順に木を巡回する。空でないグラフは連結な木であること。
    explicit eulertour(const Graph& graph, int root = 0) : tree_(graph, root) {
        first_.assign(graph.size(), -1);
        last_.assign(graph.size(), -1);
        if (graph.size() == 0) return;

        struct frame {
            int vertex;
            int next;
        };
        std::vector<frame> stack = {{root, 0}};
        append_vertex(root, 0);
        while (!stack.empty()) {
            auto& current = stack.back();
            if (current.next == static_cast<int>(graph[current.vertex].size())) {
                const int child = current.vertex;
                stack.pop_back();
                if (!stack.empty()) {
                    edge_ids_.push_back(tree_.parent_edge(child));
                    append_vertex(stack.back().vertex, static_cast<int>(stack.size()) - 1);
                }
                continue;
            }
            const auto& edge = graph[current.vertex][current.next++];
            if (edge.id == tree_.parent_edge(current.vertex)) continue;
            edge_ids_.push_back(edge.id);
            append_vertex(edge.to, static_cast<int>(stack.size()));
            stack.push_back({edge.to, 0});
        }
    }

    /// O(1)。完全Euler tour列の長さを返す。空木では0、通常は2n-1。
    int size() const noexcept {
        return static_cast<int>(vertices_.size());
    }

    /// O(1)。元の木の頂点数を返す。
    int vertex_count() const noexcept {
        return tree_.size();
    }

    /// O(1)。根を返す。空木では-1を返す。
    int root() const noexcept {
        return tree_.root();
    }

    /// O(1)。Euler tourのindex番目にいる頂点を返す。
    int operator[](int index) const {
        assert_index(index);
        return vertices_[index];
    }

    /// O(1)。index番目からindex+1番目への移動に使う辺番号を返す。
    int edge(int index) const {
        assert(0 <= index && index < size() - 1);
        return edge_ids_[index];
    }

    /// O(1)。Euler tourのindex番目にいる頂点の深さを返す。
    int depth(int index) const {
        assert_index(index);
        return depths_[index];
    }

    /// O(1)。頂点vertexがEuler tourに最初に現れる位置を返す。
    int first(int vertex) const {
        assert_vertex(vertex);
        return first_[vertex];
    }

    /// O(1)。頂点vertexがEuler tourに最後に現れる位置を返す。
    int last(int vertex) const {
        assert_vertex(vertex);
        return last_[vertex];
    }

    /// O(1)。頂点vertexのDFS行きがけ順の位置を返す。
    int index(int vertex) const {
        return tree_.index(vertex);
    }

    /// O(1)。頂点vertexの部分木を表すDFS行きがけ順の半開区間を返す。
    std::pair<int, int> subtree(int vertex) const {
        return {tree_.index(vertex), tree_.out(vertex)};
    }

    /// O(1)。DFS行きがけ順index番目の頂点を返す。
    int preorder(int index) const {
        return tree_[index];
    }

    /// O(1)。完全Euler tourの頂点列を返す。
    const std::vector<int>& vertices() const noexcept {
        return vertices_;
    }

    /// O(1)。完全Euler tourで隣り合う頂点間を移動する辺番号列を返す。
    const std::vector<int>& edges() const noexcept {
        return edge_ids_;
    }

    /// O(1)。完全Euler tourの各位置での深さを返す。
    const std::vector<int>& depths() const noexcept {
        return depths_;
    }

private:
    /// 償却O(1)。Euler tour列の末尾に頂点を追加する。
    void append_vertex(int vertex, int depth) {
        const int position = static_cast<int>(vertices_.size());
        vertices_.push_back(vertex);
        depths_.push_back(depth);
        if (first_[vertex] == -1) first_[vertex] = position;
        last_[vertex] = position;
    }

    /// O(1)。Euler tour上の位置が範囲内であることを確認する。
    void assert_index(int index) const {
        assert(0 <= index && index < size());
    }

    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < vertex_count());
    }

    rootedtree<Graph> tree_;
    std::vector<int> vertices_;
    std::vector<int> edge_ids_;
    std::vector<int> depths_;
    std::vector<int> first_;
    std::vector<int> last_;
};

}
