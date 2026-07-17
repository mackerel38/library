#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 数直線上の点集合: nearestset<long long> points; add/erase O(log n)、sum O(1)。
template<class T, class Sum = long long>
struct nearestset {
    /// O(1)。空集合を作る。
    nearestset() = default;

    /// O(log n)。valueを追加し、集合が変化したならtrueを返す。
    bool add(const T& value) {
        auto next = values_.lower_bound(value);
        if (next != values_.end() && *next == value) return false;
        const auto previous = next == values_.begin() ? values_.end() : std::prev(next);
        subtract_if(previous);
        subtract_if(next);
        const auto inserted = values_.insert(next, value);
        add_if(previous);
        add_if(inserted);
        add_if(next);
        return true;
    }

    /// O(log n)。valueを削除し、集合が変化したならtrueを返す。
    bool erase(const T& value) {
        const auto current = values_.find(value);
        if (current == values_.end()) return false;
        const auto previous = current == values_.begin() ? values_.end() : std::prev(current);
        const auto next = std::next(current);
        subtract_if(previous);
        subtract_if(current);
        subtract_if(next);
        values_.erase(current);
        add_if(previous);
        add_if(next);
        return true;
    }

    /// O(log n)。valueが含まれるなら、別の最寄り点までの距離を返す。点が一つならnullopt。
    std::optional<Sum> distance(const T& value) const {
        const auto iterator = values_.find(value);
        if (iterator == values_.end() || values_.size() < 2) return std::nullopt;
        return contribution(iterator);
    }

    /// O(1)。各点から最寄りの別点までの距離の総和を返す。点が一つ以下なら0。
    Sum sum() const noexcept { return sum_; }

    /// O(log n)。valueが含まれるならtrueを返す。
    bool contains(const T& value) const { return values_.contains(value); }

    /// O(1)。点数を返す。
    int size() const noexcept { return static_cast<int>(values_.size()); }

    /// O(1)。点を昇順に保持するsetを返す。
    const std::set<T>& values() const noexcept { return values_; }

private:
    using iterator = typename std::set<T>::const_iterator;

    Sum contribution(iterator current) const {
        assert(current != values_.end() && values_.size() >= 2);
        std::optional<Sum> result;
        if (current != values_.begin()) {
            result = static_cast<Sum>(*current) - static_cast<Sum>(*std::prev(current));
        }
        if (const auto next = std::next(current); next != values_.end()) {
            const Sum distance = static_cast<Sum>(*next) - static_cast<Sum>(*current);
            result = result ? std::min(*result, distance) : distance;
        }
        return *result;
    }

    void subtract_if(iterator iterator) {
        if (iterator != values_.end() && values_.size() >= 2) sum_ -= contribution(iterator);
    }

    void add_if(iterator iterator) {
        if (iterator != values_.end() && values_.size() >= 2) sum_ += contribution(iterator);
    }

    std::set<T> values_;
    Sum sum_{};
};

}
