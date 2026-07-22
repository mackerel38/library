#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"

namespace poe {

/// push/pop列を実行した結果。popは空stackへ残ったpop数、lengthとvalueは残る列を表す。
template <int modulus>
struct stacksequenceresult {
    int pop = 0;
    int length = 0;
    staticmodint<modulus> value = 0;

    /// O(1)。実行後の列が空ならtrueを返す。
    bool empty() const noexcept { return length == 0; }
};

/// recipe。一点更新付きpush/pop列: -1をpop、[0,base)をpushとして区間実行結果を返す。
template <int modulus, int base>
struct stacksequence {
    using mint = staticmodint<modulus>;
    using result_type = stacksequenceresult<modulus>;
    static_assert(2 <= base && base < modulus && std::gcd(base, modulus) == 1);

    /// O(n)。operationsから構築する。-1はpop、それ以外はpushするsymbolを表す。
    explicit stacksequence(const std::vector<int>& operations)
        : n_(static_cast<int>(operations.size())), power_(n_ + 1), inverse_power_(n_ + 1) {
        while (size_ < n_) size_ <<= 1;
        tree_.assign(2 * size_, {});
        power_[0] = inverse_power_[0] = 1;
        const mint inverse_base = mint(base).inv();
        for (int i = 1; i <= n_; ++i) {
            power_[i] = power_[i - 1] * base;
            inverse_power_[i] = inverse_power_[i - 1] * inverse_base;
        }
        for (int i = 0; i < n_; ++i) tree_[size_ + i] = leaf(operations[i]);
        for (int node = size_ - 1; node >= 1; --node) update(node);
    }

    /// O(1)。操作数を返す。
    int size() const noexcept { return n_; }

    /// O(log^2 n)。index番目をoperationへ置き換える。
    void set(int index, int operation) {
        assert_index(index);
        int node = size_ + index;
        tree_[node] = leaf(operation);
        while ((node >>= 1) != 0) update(node);
    }

    /// O(1)。index番目の操作を返す。popは-1で表す。
    int get(int index) const {
        assert_index(index);
        const auto& data = tree_[size_ + index];
        return data.pop ? -1 : static_cast<int>(data.value.val());
    }

    /// O(1)。operator[]でindex番目の操作を返す。
    int operator[](int index) const { return get(index); }

    /// O(log^2 n)。半開区間[left,right)を空stackから実行した正規化結果を返す。
    result_type prod(int left, int right) const {
        assert(0 <= left && left <= right && right <= n_);
        std::array<int, 64> left_nodes{}, right_nodes{};
        int left_count = 0, right_count = 0;
        left += size_;
        right += size_;
        while (left < right) {
            if (left & 1) left_nodes[left_count++] = left++;
            if (right & 1) right_nodes[right_count++] = --right;
            left >>= 1;
            right >>= 1;
        }
        result_type result;
        for (int i = 0; i < right_count; ++i) result = combine_node(right_nodes[i], result);
        for (int i = left_count - 1; i >= 0; --i) result = combine_node(left_nodes[i], result);
        return result;
    }

    /// O(1)。全操作列の実行結果を返す。
    result_type all_prod() const { return tree_[1]; }

private:
    /// O(1)。一操作を正規化する。
    result_type leaf(int operation) const {
        assert(operation == -1 || (0 <= operation && operation < base));
        if (operation == -1) return {.pop = 1};
        return {.pop = 0, .length = 1, .value = operation};
    }

    /// O(nodeの高さ)。nodeの実行後に残る列の末尾count桁を返す。
    mint suffix_value(int node, int count) const {
        assert(0 <= count && count <= tree_[node].length);
        if (count == 0) return 0;
        if (count == tree_[node].length) return tree_[node].value;
        const int left = 2 * node, right = left + 1;
        if (tree_[right].length >= count) return suffix_value(right, count);
        const int deleted = std::min(tree_[right].pop, tree_[left].length);
        const int from_left = count - tree_[right].length;
        const mint with_deleted = suffix_value(left, from_left + deleted);
        const mint deleted_value = tree_[left].value
            - (tree_[node].value - tree_[right].value)
                * inverse_power_[tree_[right].length] * power_[deleted];
        const mint kept = (with_deleted - deleted_value) * inverse_power_[deleted];
        return kept * power_[tree_[right].length] + tree_[right].value;
    }

    /// O(nodeの高さ)。tree上の左列nodeへ任意の右dataを連結した結果を返す。
    result_type combine_node(int node, const result_type& right) const {
        const auto& left = tree_[node];
        const int deleted = std::min(left.length, right.pop);
        mint kept;
        if (deleted == 0) kept = left.value;
        else if (deleted == left.length) kept = 0;
        else kept = (left.value - suffix_value(node, deleted)) * inverse_power_[deleted];
        return {
            .pop = left.pop + std::max(0, right.pop - left.length),
            .length = left.length - deleted + right.length,
            .value = kept * power_[right.length] + right.value,
        };
    }

    /// O(nodeの高さ)。nodeを左右の子から再計算する。
    void update(int node) { tree_[node] = combine_node(2 * node, tree_[2 * node + 1]); }

    /// O(1)。indexが範囲内であることを確認する。
    void assert_index(int index) const { assert(0 <= index && index < n_); }

    int n_ = 0;
    int size_ = 1;
    std::vector<result_type> tree_;
    std::vector<mint> power_;
    std::vector<mint> inverse_power_;
};

}
