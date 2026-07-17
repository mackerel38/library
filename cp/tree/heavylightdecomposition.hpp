#pragma once
#include <bits/stdc++.h>
#include "tree/rootedtree.hpp"

namespace poe {

/// HLD上の半開区間。reversedならright-1からleftへ向かう順で使う。
struct pathsegment {
    int left;
    int right;
    bool reversed;
};

/// 木のパスと部分木を列上の区間へ分解する: hld tree(graph, root); 構築O(n)。
template<undirected_graph_type Graph>
struct hld {
    /// O(n)。無向木をroot根としてHeavy-Light Decompositionする。
    explicit hld(const Graph& graph, int root = 0)
        : tree_(graph, root), head_(graph.size()), index_(graph.size()),
          vertex_(graph.size()), heavy_(graph.size(), -1), edge_index_(graph.edge_count(), -1) {
        for (int vertex = 0; vertex < graph.size(); ++vertex) {
            for (const auto& current : graph[vertex]) {
                if (tree_.parent(current.to) != vertex) continue;
                if (heavy_[vertex] == -1 ||
                    tree_.subtree_size(heavy_[vertex]) < tree_.subtree_size(current.to)) {
                    heavy_[vertex] = current.to;
                }
            }
        }
        if (graph.size() == 0) return;
        std::vector<std::pair<int, int>> stack = {{root, root}};
        int next_index = 0;
        while (!stack.empty()) {
            auto [vertex, chain_head] = stack.back();
            stack.pop_back();
            for (int current = vertex; current != -1; current = heavy_[current]) {
                head_[current] = chain_head;
                index_[current] = next_index;
                vertex_[next_index++] = current;
                if (tree_.parent_edge(current) != -1) {
                    edge_index_[tree_.parent_edge(current)] = index_[current];
                }
                for (auto iterator = graph[current].rbegin(); iterator != graph[current].rend();
                     ++iterator) {
                    const int child = iterator->to;
                    if (tree_.parent(child) == current && child != heavy_[current]) {
                        stack.emplace_back(child, child);
                    }
                }
            }
        }
    }

    /// O(log n)。二頂点の最小共通祖先を返す。
    int lca(int left, int right) const {
        assert_vertex(left);
        assert_vertex(right);
        while (head_[left] != head_[right]) {
            if (tree_.depth(head_[left]) < tree_.depth(head_[right])) std::swap(left, right);
            left = tree_.parent(head_[left]);
        }
        return tree_.depth(left) < tree_.depth(right) ? left : right;
    }

    /// O(log n)。fromからtoへの頂点列を順序付き半開区間へ分解する。
    std::vector<pathsegment> path(int from, int to) const {
        return decompose_path(from, to, false);
    }

    /// O(log n)。fromからtoへの辺列を、子頂点の位置を使う順序付き半開区間へ分解する。
    std::vector<pathsegment> edge_path(int from, int to) const {
        return decompose_path(from, to, true);
    }

    /// O(1)。頂点vertexの部分木を表す半開区間を返す。
    std::pair<int, int> subtree(int vertex) const {
        assert_vertex(vertex);
        return {index_[vertex], index_[vertex] + tree_.subtree_size(vertex)};
    }

    /// O(1)。頂点vertexの列上の位置を返す。
    int index(int vertex) const {
        assert_vertex(vertex);
        return index_[vertex];
    }

    /// O(1)。頂点vertexの列上の位置を返す。
    int operator[](int vertex) const {
        return index(vertex);
    }

    /// O(1)。列上の位置indexにある頂点を返す。
    int vertex(int index) const {
        assert(0 <= index && index < size());
        return vertex_[index];
    }

    /// O(1)。辺番号idを、その子頂点に対応する列上の位置へ変換する。
    int edge_index(int id) const {
        assert(0 <= id && id < static_cast<int>(edge_index_.size()));
        return edge_index_[id];
    }

    /// O(1)。頂点vertexの親を返す。根では-1。
    int parent(int vertex) const {
        assert_vertex(vertex);
        return tree_.parent(vertex);
    }

    /// O(1)。頂点vertexの根からの辺数を返す。
    int depth(int vertex) const {
        assert_vertex(vertex);
        return tree_.depth(vertex);
    }

    /// O(1)。頂点vertexが属するheavy pathの最上位頂点を返す。
    int head(int vertex) const {
        assert_vertex(vertex);
        return head_[vertex];
    }

    /// O(1)。根を返す。空木では-1。
    int root() const noexcept { return tree_.root(); }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return tree_.size();
    }

private:
    /// O(log n)。頂点または辺のパスを順序付き区間へ分解する。
    std::vector<pathsegment> decompose_path(int from, int to, bool edge) const {
        assert_vertex(from);
        assert_vertex(to);
        std::vector<pathsegment> left_segments;
        std::vector<pathsegment> right_segments;
        while (head_[from] != head_[to]) {
            if (tree_.depth(head_[from]) > tree_.depth(head_[to])) {
                left_segments.push_back({index_[head_[from]], index_[from] + 1, true});
                from = tree_.parent(head_[from]);
            } else {
                right_segments.push_back({index_[head_[to]], index_[to] + 1, false});
                to = tree_.parent(head_[to]);
            }
        }
        if (tree_.depth(from) > tree_.depth(to)) {
            const int left = index_[to] + static_cast<int>(edge);
            if (left < index_[from] + 1) {
                left_segments.push_back({left, index_[from] + 1, true});
            }
        } else {
            const int left = index_[from] + static_cast<int>(edge);
            if (left < index_[to] + 1) {
                right_segments.push_back({left, index_[to] + 1, false});
            }
        }
        while (!right_segments.empty()) {
            left_segments.push_back(right_segments.back());
            right_segments.pop_back();
        }
        return left_segments;
    }

    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < size());
    }

    rootedtree<Graph> tree_;
    std::vector<int> head_;
    std::vector<int> index_;
    std::vector<int> vertex_;
    std::vector<int> heavy_;
    std::vector<int> edge_index_;
};

}
