#pragma once
#include <bits/stdc++.h>

namespace poe {

/// auto [size,count]=tree.max(l,r); 区間内集合の最大要素数と達成位置数。
struct setmaxresult {
    int size = 0;
    int count = 0;
};

/// setsegtree tree(n); tree.add(l,r,x); tree.erase(l,r,x); 0<=x<64。
struct setsegtree {
    /// O(n)。n個の空集合を作る。n>=1。
    explicit setsegtree(int n) : setsegtree(std::vector<std::uint64_t>(n)) {}

    /// O(n)。各位置の初期集合をbit maskで与える。
    explicit setsegtree(const std::vector<std::uint64_t>& masks)
        : n_(static_cast<int>(masks.size())), nodes_(4 * std::max(1, n_)), lazy_(nodes_.size()) {
        assert(n_ >= 1);
        build(1, 0, n_, masks);
    }

    /// O(1)。集合数を返す。
    int size() const noexcept { return n_; }

    /// 全更新で償却O(log n)。[left,right)の全集合へelementを追加する。
    void add(int left, int right, int element) {
        assert(0 <= element && element < 64);
        add_mask(left, right, std::uint64_t{1} << element);
    }

    /// 全更新で償却O(log n)。[left,right)の全集合からelementを削除する。
    void erase(int left, int right, int element) {
        assert(0 <= element && element < 64);
        erase_mask(left, right, std::uint64_t{1} << element);
    }

    /// 全更新で償却O(log n)。[left,right)の全集合へmaskの要素を追加する。
    void add_mask(int left, int right, std::uint64_t mask) {
        apply(left, right, ~std::uint64_t{}, mask);
    }

    /// 全更新で償却O(log n)。[left,right)の全集合からmaskの要素を削除する。
    void erase_mask(int left, int right, std::uint64_t mask) {
        apply(left, right, ~mask, 0);
    }

    /// 全更新で償却O(log n)。各集合maskを(mask&keep_mask)|add_maskへ変更する。
    void apply(int left, int right, std::uint64_t keep_mask, std::uint64_t add_mask) {
        assert(0 <= left && left <= right && right <= n_);
        if (left == right) return;
        range_apply(1, 0, n_, left, right, action{keep_mask, add_mask});
    }

    /// O(log n)。[left,right)の最大要素数と達成位置数を返す。left<right。
    setmaxresult max(int left, int right) {
        assert(0 <= left && left < right && right <= n_);
        const node result = range_query(1, 0, n_, left, right);
        return {result.maximum, result.count};
    }

    /// O(log n)。positionの集合maskを返す。
    std::uint64_t get(int position) {
        assert(0 <= position && position < n_);
        return range_query(1, 0, n_, position, position + 1).any;
    }

    /// O(log n)。tree[position]で集合maskを返す。
    std::uint64_t operator[](int position) { return get(position); }

private:
    struct node {
        std::uint64_t all = 0;
        std::uint64_t any = 0;
        int maximum = 0;
        int count = 0;
    };

    struct action {
        std::uint64_t keep = ~std::uint64_t{};
        std::uint64_t add = 0;
    };

    static node merge(const node& left, const node& right) {
        node result;
        result.all = left.all & right.all;
        result.any = left.any | right.any;
        result.maximum = std::max(left.maximum, right.maximum);
        if (left.maximum == result.maximum) result.count += left.count;
        if (right.maximum == result.maximum) result.count += right.count;
        return result;
    }

    static action compose(const action& old_action, const action& new_action) {
        return {
            old_action.keep & new_action.keep,
            (old_action.add & new_action.keep) | new_action.add
        };
    }

    static bool try_apply(node& value, const action& operation) {
        const std::uint64_t changed = (~operation.keep) | operation.add;
        if (((value.any ^ value.all) & changed) != 0) return false;
        value.maximum -= std::popcount(value.all & ~operation.keep);
        value.all &= operation.keep;
        value.any &= operation.keep;
        value.maximum += std::popcount((~value.any) & operation.add);
        value.all |= operation.add;
        value.any |= operation.add;
        return true;
    }

    void build(int index, int left, int right, const std::vector<std::uint64_t>& masks) {
        if (right - left == 1) {
            nodes_[index] = {masks[left], masks[left], std::popcount(masks[left]), 1};
            return;
        }
        const int middle = (left + right) / 2;
        build(2 * index, left, middle, masks);
        build(2 * index + 1, middle, right, masks);
        pull(index);
    }

    void pull(int index) { nodes_[index] = merge(nodes_[2 * index], nodes_[2 * index + 1]); }

    void apply_node(int index, const action& operation) {
        const bool success = try_apply(nodes_[index], operation);
        assert(success);
        lazy_[index] = compose(lazy_[index], operation);
    }

    void push(int index) {
        if (lazy_[index].keep == ~std::uint64_t{} && lazy_[index].add == 0) return;
        apply_node(2 * index, lazy_[index]);
        apply_node(2 * index + 1, lazy_[index]);
        lazy_[index] = {};
    }

    void range_apply(int index, int left, int right, int query_left, int query_right,
                     const action& operation) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right && try_apply(nodes_[index], operation)) {
            lazy_[index] = compose(lazy_[index], operation);
            return;
        }
        assert(right - left >= 2);
        push(index);
        const int middle = (left + right) / 2;
        range_apply(2 * index, left, middle, query_left, query_right, operation);
        range_apply(2 * index + 1, middle, right, query_left, query_right, operation);
        pull(index);
    }

    node range_query(int index, int left, int right, int query_left, int query_right) {
        if (query_left <= left && right <= query_right) return nodes_[index];
        push(index);
        const int middle = (left + right) / 2;
        if (query_right <= middle) return range_query(2 * index, left, middle, query_left, query_right);
        if (middle <= query_left) return range_query(2 * index + 1, middle, right, query_left, query_right);
        return merge(
            range_query(2 * index, left, middle, query_left, query_right),
            range_query(2 * index + 1, middle, right, query_left, query_right)
        );
    }

    int n_;
    std::vector<node> nodes_;
    std::vector<action> lazy_;
};

}
