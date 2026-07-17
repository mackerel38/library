#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 動的中央値: median<long long> values; add/eraseはO(log n)、lower/upper/deviationはO(1)。
template <class T>
struct median {
    /// O(1)。空の多重集合を作る。
    median() = default;

    /// O(log n)。valueを一つ追加する。
    void add(const T& value) {
        if (lower_.empty() || value <= *lower_.rbegin()) {
            lower_.insert(value);
            lower_sum_ += value;
        } else {
            upper_.insert(value);
            upper_sum_ += value;
        }
        rebalance();
    }

    /// O(log n)。valueを一つ削除する。valueが存在すること。
    void erase(const T& value) {
        auto iterator = lower_.find(value);
        if (iterator != lower_.end()) {
            lower_sum_ -= value;
            lower_.erase(iterator);
        } else {
            iterator = upper_.find(value);
            assert(iterator != upper_.end());
            upper_sum_ -= value;
            upper_.erase(iterator);
        }
        rebalance();
    }

    /// O(1)。要素数を返す。
    int size() const noexcept { return static_cast<int>(lower_.size() + upper_.size()); }

    /// O(1)。空かを返す。
    bool empty() const noexcept { return lower_.empty(); }

    /// O(1)。小さい側の中央値を返す。空でないこと。
    const T& lower() const {
        assert(!empty());
        return *lower_.rbegin();
    }

    /// O(1)。大きい側の中央値を返す。空でないこと。要素数が奇数ならlower()と等しい。
    const T& upper() const {
        assert(!empty());
        return lower_.size() > upper_.size() ? *lower_.rbegin() : *upper_.begin();
    }

    /// O(1)。lower()から各要素までの絶対差の総和を返す。空なら0。
    T deviation() const {
        if (empty()) return T{};
        const T center = lower();
        return center * static_cast<T>(lower_.size()) - lower_sum_ +
               upper_sum_ - center * static_cast<T>(upper_.size());
    }

private:
    void move_to_lower() {
        const auto iterator = upper_.begin();
        lower_sum_ += *iterator;
        upper_sum_ -= *iterator;
        lower_.insert(*iterator);
        upper_.erase(iterator);
    }

    void move_to_upper() {
        const auto iterator = std::prev(lower_.end());
        upper_sum_ += *iterator;
        lower_sum_ -= *iterator;
        upper_.insert(*iterator);
        lower_.erase(iterator);
    }

    void rebalance() {
        while (lower_.size() < upper_.size()) move_to_lower();
        while (lower_.size() > upper_.size() + 1) move_to_upper();
    }

    std::multiset<T> lower_;
    std::multiset<T> upper_;
    T lower_sum_{};
    T upper_sum_{};
};

}
