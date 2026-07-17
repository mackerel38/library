#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// Static top treeのcluster生成操作。
enum class statictoptree_type { vertex, compress, rake, add_edge, add_vertex };

/// 根付き木を深さO(log n)のcluster merge木へ変換する。構築O(n log n)。
struct statictoptree {
    /// O(n log n)。無向木をrootで根付けてStatic top treeを構築する。
    template <undirected_graph_type Graph>
    explicit statictoptree(const Graph& graph, int root = 0) {
        const int n = graph.size();
        if (n == 0) {
            assert(root == 0);
            return;
        }
        assert(0 <= root && root < n && graph.edge_count() == n - 1);
        children_.resize(n);
        std::vector<int> parent(n, -2), order{root};
        parent[root] = -1;
        for (std::size_t index = 0; index < order.size(); ++index) {
            const int vertex = order[index];
            for (const auto& edge : graph[vertex]) {
                if (edge.to == parent[vertex]) continue;
                assert(parent[edge.to] == -2);
                parent[edge.to] = vertex;
                children_[vertex].push_back(edge.to);
                order.push_back(edge.to);
            }
        }
        assert(static_cast<int>(order.size()) == n);
        std::vector<int> subtree(n, 1);
        for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
            const int vertex = *iterator;
            for (const int child : children_[vertex]) subtree[vertex] += subtree[child];
            if (!children_[vertex].empty()) {
                const auto heavy = std::max_element(
                    children_[vertex].begin(), children_[vertex].end(),
                    [&](int left, int right) { return subtree[left] < subtree[right]; }
                );
                std::iter_swap(children_[vertex].begin(), heavy);
            }
        }
        parent_.assign(n, -1);
        left_.assign(n, -1);
        right_.assign(n, -1);
        vertex_.resize(n);
        std::iota(vertex_.begin(), vertex_.end(), 0);
        type_.assign(n, statictoptree_type::vertex);
        cluster_root_ = compress_path(root).first;
        children_.clear();
        children_.shrink_to_fit();
    }

    /// O(1)。元の木の頂点数を返す。
    int vertex_count() const noexcept { return static_cast<int>(vertex_.size()) - operator_count_; }

    /// O(1)。cluster merge木のnode数を返す。
    int size() const noexcept { return static_cast<int>(type_.size()); }

    /// O(1)。cluster merge木の根を返す。空木では-1。
    int root() const noexcept { return cluster_root_; }

    /// O(1)。nodeの親、なければ-1を返す。
    int parent(int node) const { return parent_[node]; }

    /// O(1)。nodeの左の子、なければ-1を返す。
    int left(int node) const { return left_[node]; }

    /// O(1)。nodeの右の子、なければ-1を返す。
    int right(int node) const { return right_[node]; }

    /// O(1)。nodeの生成操作を返す。
    statictoptree_type type(int node) const { return type_[node]; }

    /// O(1)。vertex/add_vertex nodeに対応する元頂点を返す。それ以外は-1。
    int vertex(int node) const { return vertex_[node]; }

private:
    using weighted_node = std::pair<int, int>;

    int add_operator(int left, int right, statictoptree_type type) {
        const int node = size();
        parent_.push_back(-1);
        left_.push_back(left);
        right_.push_back(right);
        vertex_.push_back(-1);
        type_.push_back(type);
        ++operator_count_;
        if (left != -1) parent_[left] = node;
        if (right != -1) parent_[right] = node;
        return node;
    }

    void set_vertex_node(int vertex, int left, statictoptree_type type) {
        parent_[vertex] = -1;
        left_[vertex] = left;
        right_[vertex] = -1;
        type_[vertex] = type;
        if (left != -1) parent_[left] = vertex;
    }

    weighted_node merge_balanced(const std::vector<weighted_node>& nodes, statictoptree_type type) {
        assert(!nodes.empty());
        if (nodes.size() == 1) return nodes[0];
        int balance = 0;
        for (const auto [node, weight] : nodes) { (void)node; balance += weight; }
        std::vector<weighted_node> left_nodes, right_nodes;
        for (const auto item : nodes) {
            (balance > item.second ? left_nodes : right_nodes).push_back(item);
            balance -= item.second * 2;
        }
        assert(!left_nodes.empty() && !right_nodes.empty());
        const auto left = merge_balanced(left_nodes, type);
        const auto right = merge_balanced(right_nodes, type);
        return {add_operator(left.first, right.first, type), left.second + right.second};
    }

    weighted_node compress_path(int vertex) {
        std::vector<weighted_node> path{add_vertex(vertex)};
        while (!children_[vertex].empty()) {
            vertex = children_[vertex][0];
            path.push_back(add_vertex(vertex));
        }
        return merge_balanced(path, statictoptree_type::compress);
    }

    weighted_node rake_children(int vertex) {
        std::vector<weighted_node> nodes;
        for (int index = 1; index < static_cast<int>(children_[vertex].size()); ++index) {
            nodes.push_back(add_edge(children_[vertex][index]));
        }
        if (nodes.empty()) return {-1, 0};
        return merge_balanced(nodes, statictoptree_type::rake);
    }

    weighted_node add_edge(int vertex) {
        const auto child = compress_path(vertex);
        return {add_operator(child.first, -1, statictoptree_type::add_edge), child.second};
    }

    weighted_node add_vertex(int vertex) {
        const auto children = rake_children(vertex);
        set_vertex_node(
            vertex, children.first,
            children.first == -1 ? statictoptree_type::vertex : statictoptree_type::add_vertex
        );
        return {vertex, children.second + 1};
    }

    int cluster_root_ = -1;
    int operator_count_ = 0;
    std::vector<std::vector<int>> children_;
    std::vector<int> parent_;
    std::vector<int> left_;
    std::vector<int> right_;
    std::vector<int> vertex_;
    std::vector<statictoptree_type> type_;
};

/// Static top treeへ5演算の木DPを載せ、頂点更新をO(log n)で再計算する。
template <class Path, class Point, class Vertex, class Compress, class Rake, class AddEdge, class AddVertex>
struct statictoptreedp {
    statictoptreedp(
        const statictoptree& tree,
        Vertex vertex,
        Compress compress,
        Rake rake,
        AddEdge add_edge,
        AddVertex add_vertex
    ) : tree_(&tree), vertex_(std::move(vertex)), compress_(std::move(compress)),
        rake_(std::move(rake)), add_edge_(std::move(add_edge)), add_vertex_(std::move(add_vertex)),
        path_(tree.size()), point_(tree.size()) {
        if (tree.root() == -1) return;
        std::vector<std::pair<int, bool>> stack{{tree.root(), false}};
        while (!stack.empty()) {
            const auto [node, visited] = stack.back();
            stack.pop_back();
            if (visited) { recalculate(node); continue; }
            stack.push_back({node, true});
            if (tree.right(node) != -1) stack.push_back({tree.right(node), false});
            if (tree.left(node) != -1) stack.push_back({tree.left(node), false});
        }
    }

    /// O(log n)。元頂点vertexの外部値を変更した後、影響するclusterを再計算する。
    void update(int vertex) {
        assert(0 <= vertex && vertex < tree_->vertex_count());
        for (int node = vertex; node != -1; node = tree_->parent(node)) recalculate(node);
    }

    /// O(1)。木全体のpath cluster値を返す。
    const Path& get() const {
        assert(tree_->root() != -1);
        return path_[tree_->root()];
    }

private:
    void recalculate(int node) {
        switch (tree_->type(node)) {
            case statictoptree_type::vertex:
                path_[node] = vertex_(tree_->vertex(node));
                break;
            case statictoptree_type::compress:
                path_[node] = compress_(path_[tree_->left(node)], path_[tree_->right(node)]);
                break;
            case statictoptree_type::rake:
                point_[node] = rake_(point_[tree_->left(node)], point_[tree_->right(node)]);
                break;
            case statictoptree_type::add_edge:
                point_[node] = add_edge_(path_[tree_->left(node)]);
                break;
            case statictoptree_type::add_vertex:
                path_[node] = add_vertex_(point_[tree_->left(node)], tree_->vertex(node));
                break;
        }
    }

    const statictoptree* tree_;
    Vertex vertex_;
    Compress compress_;
    Rake rake_;
    AddEdge add_edge_;
    AddVertex add_vertex_;
    std::vector<Path> path_;
    std::vector<Point> point_;
};

/// O(n)。make_statictoptreedp(tree, vertex, compress, rake, add_edge, add_vertex)でDPを構築する。
template <class Path, class Point, class Vertex, class Compress, class Rake, class AddEdge, class AddVertex>
auto make_statictoptreedp(
    const statictoptree& tree,
    Vertex vertex,
    Compress compress,
    Rake rake,
    AddEdge add_edge,
    AddVertex add_vertex
) {
    return statictoptreedp<Path, Point, Vertex, Compress, Rake, AddEdge, AddVertex>(
        tree, std::move(vertex), std::move(compress), std::move(rake),
        std::move(add_edge), std::move(add_vertex)
    );
}

}
