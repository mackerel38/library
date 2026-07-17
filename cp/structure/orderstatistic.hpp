#pragma once
#include <bits/stdc++.h>
#include "structure/fenwicktree.hpp"

namespace poe {

/// 座標圧縮済み動的順序統計multiset: orderstatisticmultiset<long long> data(universe)。
template <class T>
struct orderstatisticmultiset {
    /// O(u log u)。追加され得る全値universeから空のmultisetを構築する。
    explicit orderstatisticmultiset(std::vector<T> universe)
        : values_(std::move(universe)), counts_(prepare(values_)), sums_(values_.size()) {}

    /// O(log u)。valueをcount個追加する。
    void add(const T& value, long long count = 1) {
        assert(count >= 0);
        const int index = index_of(value);
        counts_.add(index, count);
        sums_.add(index, value * count);
        size_ += count;
        sum_ += value * count;
    }

    /// O(log u)。valueをcount個削除する。十分な個数が存在すること。
    void erase(const T& value, long long count = 1) {
        assert(count >= 0);
        const int index = index_of(value);
        assert(counts_[index] >= count);
        counts_.add(index, -count);
        sums_.add(index, -(value * count));
        size_ -= count;
        sum_ -= value * count;
    }

    /// O(log u)。valueの個数を返す。
    long long count(const T& value) const {
        const auto iterator = std::lower_bound(values_.begin(), values_.end(), value);
        if (iterator == values_.end() || *iterator != value) return 0;
        return counts_[static_cast<int>(iterator - values_.begin())];
    }

    /// O(log u)。value未満の要素数を返す。
    long long count_less(const T& value) const {
        const int index = std::lower_bound(values_.begin(), values_.end(), value) - values_.begin();
        return counts_.sum(index);
    }

    /// O(log u)。value未満の要素の総和を返す。
    T sum_less(const T& value) const {
        const int index = std::lower_bound(values_.begin(), values_.end(), value) - values_.begin();
        return sums_.sum(index);
    }

    /// O(log u)。全要素へmax(lower,min(upper,x))を適用した総和を返す。lower>upperならlower*size。
    T clamp_sum(const T& lower, const T& upper) const {
        if (lower > upper) return lower * size_;
        const int lower_index = std::lower_bound(values_.begin(), values_.end(), lower) - values_.begin();
        const int upper_index = std::upper_bound(values_.begin(), values_.end(), upper) - values_.begin();
        const long long lower_count = counts_.sum(lower_index);
        const long long upper_count = size_ - counts_.sum(upper_index);
        return lower * lower_count
             + (sums_.sum(upper_index) - sums_.sum(lower_index))
             + upper * upper_count;
    }

    /// O(log u)。0-indexedでindex番目に小さい値を返す。
    T kth(long long index) const {
        assert(0 <= index && index < size_);
        return values_[counts_.lower_bound(index + 1)];
    }

    /// O(log u)。小さい方からcount個の総和を返す。
    T sum_smallest(long long count) const {
        assert(0 <= count && count <= size_);
        if (count == 0) return T{};
        const int index = counts_.lower_bound(count);
        const long long before = counts_.sum(index);
        return sums_.sum(index) + values_[index] * (count - before);
    }

    /// O(log u)。大きい方からcount個の総和を返す。
    T sum_largest(long long count) const {
        assert(0 <= count && count <= size_);
        return sum_ - sum_smallest(size_ - count);
    }

    /// O(1)。要素数を返す。
    long long size() const noexcept {
        return size_;
    }

    /// O(1)。空ならtrueを返す。
    bool empty() const noexcept {
        return size_ == 0;
    }

    /// O(1)。全要素の総和を返す。
    T sum() const {
        return sum_;
    }

private:
    static int prepare(std::vector<T>& values) {
        std::ranges::sort(values);
        values.erase(std::unique(values.begin(), values.end()), values.end());
        return static_cast<int>(values.size());
    }

    int index_of(const T& value) const {
        const auto iterator = std::lower_bound(values_.begin(), values_.end(), value);
        assert(iterator != values_.end() && *iterator == value);
        return static_cast<int>(iterator - values_.begin());
    }

    std::vector<T> values_;
    fenwicktree<long long> counts_;
    fenwicktree<T> sums_;
    long long size_ = 0;
    T sum_{};
};

}
