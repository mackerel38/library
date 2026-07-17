#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 永続Segment Tree: persistentsegtree<S,op,e> tree(values); version root=tree.initial()。
template <class S, auto op, auto e>
struct persistentsegtree {
    using version = int;

    /// O(n)。n個のe()から初期versionを構築する。
    explicit persistentsegtree(int n) : size_(n) {
        assert(n >= 0);
        std::vector<S> values(n, e());
        initial_ = build(values, 0, n);
    }

    /// O(n)。valuesから初期versionを構築する。
    explicit persistentsegtree(const std::vector<S>& values)
        : size_(static_cast<int>(values.size())) {
        initial_ = build(values, 0, size_);
    }

    /// O(1)。構築時のversionを返す。
    version initial() const noexcept { return initial_; }

    /// O(log n)。rootのa[index]をvalueへ置き換えた新しいversionを返す。
    version set(version root, int index, const S& value) {
        assert(0 <= index && index < size_);
        assert_root(root);
        return set_impl(root, 0, size_, index, value);
    }

    /// O(log n)。rootのa[index]をop(a[index],value)へ更新した新versionを返す。
    version apply(version root, int index, const S& value) {
        return set(root, index, op(get(root, index), value));
    }

    /// O(log n)。rootのa[index]を返す。
    S get(version root, int index) const {
        assert(0 <= index && index < size_);
        assert_root(root);
        int left = 0, right = size_;
        while (left + 1 < right) {
            const int middle = std::midpoint(left, right);
            if (index < middle) {
                root = nodes_[root].left;
                right = middle;
            } else {
                root = nodes_[root].right;
                left = middle;
            }
        }
        return nodes_[root].value;
    }

    /// O(log n)。rootの半開区間[left,right)の積を返す。
    S prod(version root, int left, int right) const {
        assert(0 <= left && left <= right && right <= size_);
        if (left == right) return e();
        assert_root(root);
        return prod_impl(root, 0, size_, left, right);
    }

    /// O(1)。rootの全要素の積を返す。空配列ではe()。
    S all_prod(version root) const {
        if (size_ == 0) return e();
        assert_root(root);
        return nodes_[root].value;
    }

    /// O(1)。配列長を返す。
    int size() const noexcept { return size_; }

    /// O(1)。現在確保済みnode数を返す。
    int node_count() const noexcept { return static_cast<int>(nodes_.size()); }

    /// O(1)。今後追加するnode領域を予約する。
    void reserve_nodes(int count) {
        assert(count >= 0);
        nodes_.reserve(count);
    }

private:
    struct node {
        S value;
        int left = -1;
        int right = -1;
    };

    version make_node(const S& value, int left = -1, int right = -1) {
        nodes_.push_back({value, left, right});
        return static_cast<int>(nodes_.size()) - 1;
    }

    version build(const std::vector<S>& values, int left, int right) {
        if (left == right) return -1;
        if (left + 1 == right) return make_node(values[left]);
        const int middle = std::midpoint(left, right);
        const int left_root = build(values, left, middle);
        const int right_root = build(values, middle, right);
        return make_node(op(nodes_[left_root].value, nodes_[right_root].value),
                         left_root, right_root);
    }

    version set_impl(version root, int left, int right, int index, const S& value) {
        if (left + 1 == right) return make_node(value);
        const int middle = std::midpoint(left, right);
        int left_root = nodes_[root].left;
        int right_root = nodes_[root].right;
        if (index < middle) left_root = set_impl(left_root, left, middle, index, value);
        else right_root = set_impl(right_root, middle, right, index, value);
        return make_node(op(nodes_[left_root].value, nodes_[right_root].value),
                         left_root, right_root);
    }

    S prod_impl(version root, int left, int right,
                int query_left, int query_right) const {
        if (query_right <= left || right <= query_left) return e();
        if (query_left <= left && right <= query_right) return nodes_[root].value;
        const int middle = std::midpoint(left, right);
        return op(prod_impl(nodes_[root].left, left, middle, query_left, query_right),
                  prod_impl(nodes_[root].right, middle, right, query_left, query_right));
    }

    void assert_root(version root) const {
        assert(size_ == 0 ? root == -1 : 0 <= root && root < node_count());
    }

    int size_ = 0;
    version initial_ = -1;
    std::vector<node> nodes_;
};

}
