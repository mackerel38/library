#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 小alphabetの各位置の許容文字を更新し、異なる非空部分列数を管理する。
/// 構築O(n A^3)、set O(A^3 log n)、count O(A)、領域O(n A^2)。
template<class T, int Alphabet>
struct dynamicsubsequencecount {
    static_assert(1 <= Alphabet && Alphabet <= 64);
    using mask_type = std::uint64_t;

    /// O(n A^3)。allowed[i]の立っているbitを位置iで選べる文字として構築する。
    explicit dynamicsubsequencecount(const std::vector<mask_type>& allowed)
        : n_(static_cast<int>(allowed.size())), allowed_(allowed) {
        while (tree_size_ < n_) tree_size_ <<= 1;
        tree_.assign(2 * tree_size_, identity());
        for (int index = 0; index < n_; ++index) {
            assert_valid_mask(allowed_[index]);
            tree_[tree_size_ + index] = transition(allowed_[index]);
        }
        for (int node = tree_size_ - 1; node >= 1; --node) update(node);
    }

    /// O(1)。位置数を返す。
    int size() const noexcept { return n_; }

    /// O(A^3 log n)。位置indexで選べる文字集合をmaskへ変更する。
    void set(int index, mask_type mask) {
        assert_index(index);
        assert_valid_mask(mask);
        allowed_[index] = mask;
        int node = tree_size_ + index;
        tree_[node] = transition(mask);
        while ((node >>= 1) >= 1) update(node);
    }

    /// O(1)。位置indexで選べる文字集合のbit maskを返す。
    mask_type operator[](int index) const {
        assert_index(index);
        return allowed_[index];
    }

    /// O(A)。現在作れる相異なる非空部分列の個数を返す。
    T count() const {
        T result{};
        for (int symbol = 0; symbol < Alphabet; ++symbol) {
            result += tree_[1][symbol][Alphabet];
        }
        return result;
    }

private:
    static constexpr int dimension = Alphabet + 1;
    using transformation = std::array<std::array<T, dimension>, dimension>;

    static transformation identity() {
        transformation result{};
        for (int index = 0; index < dimension; ++index) result[index][index] = T{1};
        return result;
    }

    static transformation transition(mask_type mask) {
        transformation result = identity();
        for (int symbol = 0; symbol < Alphabet; ++symbol) {
            if ((mask >> symbol) & 1U) {
                result[symbol].fill(T{1});
            }
        }
        return result;
    }

    static transformation compose(
        const transformation& first,
        const transformation& second
    ) {
        transformation result{};
        for (int row = 0; row < dimension; ++row) {
            for (int middle = 0; middle < dimension; ++middle) {
                for (int column = 0; column < dimension; ++column) {
                    result[row][column] += second[row][middle] * first[middle][column];
                }
            }
        }
        return result;
    }

    void update(int node) {
        tree_[node] = compose(tree_[2 * node], tree_[2 * node + 1]);
    }

    void assert_index(int index) const { assert(0 <= index && index < n_); }

    static void assert_valid_mask(mask_type mask) {
        if constexpr (Alphabet < 64) assert(mask < (mask_type{1} << Alphabet));
    }

    int n_ = 0;
    int tree_size_ = 1;
    std::vector<mask_type> allowed_;
    std::vector<transformation> tree_;
};

}  // namespace poe
