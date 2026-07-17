#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 冪等演算の静的区間積: sparsetable<int, op> table(values); 構築O(n log n)、取得O(1)。
template <class S, auto op>
struct sparsetable {
    /// O(n log n)。valuesから構築する。opは結合的かつ冪等であること。
    explicit sparsetable(const std::vector<S>& values) : values_(values) {
        const int levels = values.empty() ? 0 : std::bit_width(values.size());
        table_.assign(levels, values);
        for (int level = 1; level < levels; ++level) {
            const int length = 1 << level;
            for (int left = 0; left + length <= size(); ++left) {
                table_[level][left] =
                    op(table_[level - 1][left], table_[level - 1][left + length / 2]);
            }
        }
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return static_cast<int>(values_.size());
    }

    /// O(1)。table[index]で元の列の要素を返す。
    const S& operator[](int index) const {
        assert(0 <= index && index < size());
        return values_[index];
    }

    /// O(1)。table.prod(left, right)で非空な[left, right)の積を返す。
    S prod(int left, int right) const {
        assert(0 <= left && left < right && right <= size());
        const int level = std::bit_width(static_cast<unsigned>(right - left)) - 1;
        return op(table_[level][left], table_[level][right - (1 << level)]);
    }

private:
    std::vector<S> values_;
    std::vector<std::vector<S>> table_;
};

/// 一般モノイドの静的区間積: disjointsparsetable<S, op> table(values); 構築O(n log n)、取得O(1)。
template <class S, auto op>
struct disjointsparsetable {
    /// O(n log n)。valuesから構築する。opは結合的であること。
    explicit disjointsparsetable(const std::vector<S>& values) : values_(values) {
        const int levels = values.size() <= 1 ? 0 : std::bit_width(values.size() - 1);
        table_.assign(levels, values);
        for (int level = 0; level < levels; ++level) {
            const int half = 1 << level;
            const int block = half << 1;
            for (int begin = 0; begin < size(); begin += block) {
                const int middle = std::min(begin + half, size());
                const int end = std::min(begin + block, size());
                if (begin < middle) {
                    table_[level][middle - 1] = values_[middle - 1];
                    for (int index = middle - 2; index >= begin; --index) {
                        table_[level][index] = op(values_[index], table_[level][index + 1]);
                    }
                }
                if (middle < end) {
                    table_[level][middle] = values_[middle];
                    for (int index = middle + 1; index < end; ++index) {
                        table_[level][index] = op(table_[level][index - 1], values_[index]);
                    }
                }
            }
        }
    }

    /// O(1)。要素数を返す。
    int size() const noexcept {
        return static_cast<int>(values_.size());
    }

    /// O(1)。table[index]で元の列の要素を返す。
    const S& operator[](int index) const {
        assert(0 <= index && index < size());
        return values_[index];
    }

    /// O(1)。table.prod(left, right)で非空な[left, right)の積を返す。
    S prod(int left, int right) const {
        assert(0 <= left && left < right && right <= size());
        --right;
        if (left == right) {
            return values_[left];
        }
        const int level = std::bit_width(static_cast<unsigned>(left ^ right)) - 1;
        return op(table_[level][left], table_[level][right]);
    }

private:
    std::vector<S> values_;
    std::vector<std::vector<S>> table_;
};

}
