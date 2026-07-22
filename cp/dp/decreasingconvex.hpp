#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 非負整数上の単調減少な離散凸関数を折れ点で持つ: decreasingconvexsequence<long long> f。
template <class T>
struct decreasingconvexsequence {
    /// O(1)。f(x)=0 (x>=0)を作る。
    decreasingconvexsequence() = default;

    /// O(1)。f(x)=cost*max(required-x,0) (x>=0)を作る。
    decreasingconvexsequence(long long required, T cost)
        : value_zero_(static_cast<T>(required) * cost),
          difference_zero_(required > 0 ? cost : T{}) {
        assert(required >= 0 && cost >= T{});
        if (required > 0 && cost != T{}) changes_[required] = cost;
    }

    /// O(smaller log(larger))。f(x)へother(x)を点ごとに加える。
    void merge(decreasingconvexsequence other) {
        value_zero_ += other.value_zero_;
        difference_zero_ += other.difference_zero_;
        if (changes_.size() < other.changes_.size()) changes_.swap(other.changes_);
        for (const auto& [position, decrease] : other.changes_) {
            changes_[position] += decrease;
        }
    }

    /// ならしO(p log p)。g(x)=min_{y>=max(x,required)}((y-x)cost+f(y))へ更新する。
    void suffix_min(T cost, long long required) {
        assert(required >= 0 && cost >= T{});
        if (required == 0 && cost >= difference_zero_) return;
        long long position = 0;
        while (!changes_.empty()) {
            const auto iterator = changes_.begin();
            if (iterator->first > required && difference_zero_ < cost) break;
            value_zero_ -= static_cast<T>(iterator->first - position) *
                           difference_zero_;
            position = iterator->first;
            difference_zero_ -= iterator->second;
            changes_.erase(iterator);
        }
        if (position < required) {
            value_zero_ -= static_cast<T>(required - position) *
                           difference_zero_;
            position = required;
        }
        if (position > 0 && cost != difference_zero_) {
            changes_[position] += cost - difference_zero_;
        }
        value_zero_ += static_cast<T>(position) * cost;
        difference_zero_ = cost;
    }

    /// O(p)。非負整数xにおけるf(x)を返す。
    T operator[](long long x) const {
        assert(x >= 0);
        T value = value_zero_;
        T difference = difference_zero_;
        long long position = 0;
        for (const auto& [next, decrease] : changes_) {
            if (x <= next) break;
            value -= static_cast<T>(next - position) * difference;
            position = next;
            difference -= decrease;
        }
        value -= static_cast<T>(x - position) * difference;
        return value;
    }

    /// O(1)。f(0)を返す。
    T zero() const { return value_zero_; }

    /// O(1)。保持している傾き変化点数を返す。
    int pieces() const noexcept { return static_cast<int>(changes_.size()); }

private:
    T value_zero_{};
    T difference_zero_{};
    std::map<long long, T> changes_;
};

}
