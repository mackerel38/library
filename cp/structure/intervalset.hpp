#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 互いに素な半開区間の和集合: intervalset<long long> covered; add/eraseで被覆長も管理する。
template <class T = long long>
struct intervalset {
    static_assert(std::is_arithmetic_v<T>);

    /// O(1)。空の区間集合を作る。
    intervalset() = default;

    /// O((k+1)log n)。[left,right)を加え、新しく加わった長さを返す。kは吸収する区間数。
    T add(T left, T right) {
        assert(left <= right);
        if (left == right) return T{};
        const T old = covered_;
        auto iterator = intervals_.lower_bound(left);
        if (iterator != intervals_.begin()) {
            auto previous = std::prev(iterator);
            if (previous->second >= left) iterator = previous;
        }
        while (iterator != intervals_.end() && iterator->first <= right) {
            left = std::min(left, iterator->first);
            right = std::max(right, iterator->second);
            covered_ -= iterator->second - iterator->first;
            iterator = intervals_.erase(iterator);
        }
        intervals_[left] = right;
        covered_ += right - left;
        return covered_ - old;
    }

    /// O((k+1)log n)。[left,right)を除き、実際に除かれた長さを返す。kは交差する区間数。
    T erase(T left, T right) {
        assert(left <= right);
        if (left == right) return T{};
        const T old = covered_;
        auto iterator = intervals_.lower_bound(left);
        if (iterator != intervals_.begin()) --iterator;
        std::vector<std::pair<T, T>> remains;
        while (iterator != intervals_.end()) {
            const auto [begin, end] = *iterator;
            if (end <= left) {
                ++iterator;
                continue;
            }
            if (begin >= right) break;
            covered_ -= end - begin;
            iterator = intervals_.erase(iterator);
            if (begin < left) remains.emplace_back(begin, left);
            if (right < end) remains.emplace_back(right, end);
        }
        for (const auto& [begin, end] : remains) {
            intervals_[begin] = end;
            covered_ += end - begin;
        }
        return old - covered_;
    }

    /// O(log n)。valueが和集合に含まれるか返す。
    bool contains(T value) const {
        auto iterator = intervals_.upper_bound(value);
        if (iterator == intervals_.begin()) return false;
        --iterator;
        return value < iterator->second;
    }

    /// O(1)。和集合の長さを返す。
    T covered() const noexcept {
        return covered_;
    }

    /// O(1)。互いに交わらず隣接もしない半開区間のmapを返す。
    const std::map<T, T>& intervals() const noexcept {
        return intervals_;
    }

private:
    std::map<T, T> intervals_;
    T covered_{};
};

}
