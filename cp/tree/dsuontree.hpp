#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 部分木情報を使い回すDSU on Tree: dsuontree tree(parent); run(add,remove,query)。
struct dsuontree {
    /// O(n)。parent[root]=-1である根付き木を前処理する。
    explicit dsuontree(std::vector<int> parent)
        : parent_(std::move(parent)), children_(parent_.size()),
          in_(parent_.size()), out_(parent_.size()), subtree_size_(parent_.size(), 1),
          heavy_(parent_.size(), -1) {
        const int n = size();
        if (n == 0) return;
        int root_count = 0;
        for (int vertex = 0; vertex < n; ++vertex) {
            if (parent_[vertex] == -1) {
                root_ = vertex;
                ++root_count;
            } else {
                assert(0 <= parent_[vertex] && parent_[vertex] < n);
                children_[parent_[vertex]].push_back(vertex);
            }
        }
        assert(root_count == 1);

        std::vector<int> stack = {root_};
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            in_[vertex] = static_cast<int>(order_.size());
            order_.push_back(vertex);
            for (auto iterator = children_[vertex].rbegin();
                 iterator != children_[vertex].rend(); ++iterator) {
                stack.push_back(*iterator);
            }
        }
        assert(static_cast<int>(order_.size()) == n);
        for (auto iterator = order_.rbegin(); iterator != order_.rend(); ++iterator) {
            const int vertex = *iterator;
            int best_size = 0;
            for (int child : children_[vertex]) {
                subtree_size_[vertex] += subtree_size_[child];
                if (subtree_size_[child] > best_size) {
                    best_size = subtree_size_[child];
                    heavy_[vertex] = child;
                }
            }
            out_[vertex] = in_[vertex] + subtree_size_[vertex];
        }
    }

    /// O(n log n)回のcallback。query(v)時に表はvの部分木だけを含み、終了時は空になる。
    template <class Add, class Remove, class Query>
    void run(Add add, Remove remove, Query query) const {
        if (size() == 0) return;
        enum action_type { solve, add_subtree, add_vertex, ask, remove_subtree };
        struct action {
            action_type type;
            int vertex;
            bool keep = false;
        };
        std::vector<action> stack = {{solve, root_, false}};
        while (!stack.empty()) {
            const action current = stack.back();
            stack.pop_back();
            const int vertex = current.vertex;
            if (current.type == add_subtree || current.type == remove_subtree) {
                for (int i = in_[vertex]; i < out_[vertex]; ++i) {
                    if (current.type == add_subtree) add(order_[i]);
                    else remove(order_[i]);
                }
                continue;
            }
            if (current.type == add_vertex) {
                add(vertex);
                continue;
            }
            if (current.type == ask) {
                query(vertex);
                continue;
            }

            if (!current.keep) stack.push_back({remove_subtree, vertex, false});
            stack.push_back({ask, vertex, false});
            stack.push_back({add_vertex, vertex, false});
            for (auto iterator = children_[vertex].rbegin();
                 iterator != children_[vertex].rend(); ++iterator) {
                if (*iterator != heavy_[vertex]) {
                    stack.push_back({add_subtree, *iterator, false});
                }
            }
            if (heavy_[vertex] != -1) stack.push_back({solve, heavy_[vertex], true});
            for (auto iterator = children_[vertex].rbegin();
                 iterator != children_[vertex].rend(); ++iterator) {
                if (*iterator != heavy_[vertex]) stack.push_back({solve, *iterator, false});
            }
        }
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept { return static_cast<int>(parent_.size()); }

    /// O(1)。根を返す。空木では-1。
    int root() const noexcept { return root_; }

    /// O(1)。頂点vertexの親を返す。根では-1。
    int parent(int vertex) const { return parent_[vertex]; }

    /// O(1)。頂点vertexのheavy childを返す。葉では-1。
    int heavy_child(int vertex) const { return heavy_[vertex]; }

    /// O(1)。頂点vertexの部分木サイズを返す。
    int subtree_size(int vertex) const { return subtree_size_[vertex]; }

    /// O(1)。頂点vertexのEuler tour上の位置を返す。
    int index(int vertex) const { return in_[vertex]; }

    /// O(1)。頂点vertexの部分木区間[index(vertex),out(vertex))の右端を返す。
    int out(int vertex) const { return out_[vertex]; }

    /// O(1)。Euler tour上のindex番目の頂点を返す。
    int operator[](int index) const { return order_[index]; }

private:
    int root_ = -1;
    std::vector<int> parent_;
    std::vector<std::vector<int>> children_;
    std::vector<int> in_;
    std::vector<int> out_;
    std::vector<int> subtree_size_;
    std::vector<int> heavy_;
    std::vector<int> order_;
};

}
