#pragma once
#include <bits/stdc++.h>

namespace poe {

/// intervalmapの区間片。半開区間[left,right)の値がvalueである。
template<class Coordinate, class Value>
struct intervalmapsegment {
    Coordinate left;
    Coordinate right;
    Value value;
};

/// 区間定数な写像: intervalmap<long long,int> data(left,right,initial); assignは置換前の区間片を返す。
template<class Coordinate, class Value>
struct intervalmap {
    using segment = intervalmapsegment<Coordinate, Value>;

    /// O(1)。[left,right)をinitialで初期化する。left<right。
    intervalmap(Coordinate left, Coordinate right, Value initial)
        : domain_left_(left), domain_right_(right) {
        assert(left < right);
        intervals_.emplace(left, node{right, std::move(initial)});
    }

    /// O((k+1)log n)。[left,right)をvalueへ代入し、置換前のk個の区間片を返す。
    std::vector<segment> assign(Coordinate left, Coordinate right, const Value& value) {
        assert(domain_left_ <= left && left <= right && right <= domain_right_);
        if (left == right) return {};
        auto right_iterator = split(right);
        auto left_iterator = split(left);

        std::vector<segment> removed;
        removed.reserve(std::distance(left_iterator, right_iterator));
        for (auto iterator = left_iterator; iterator != right_iterator; ++iterator) {
            removed.push_back({iterator->first, iterator->second.right, iterator->second.value});
        }
        intervals_.erase(left_iterator, right_iterator);
        auto current = intervals_.emplace(left, node{right, value}).first;

        if (current != intervals_.begin()) {
            auto previous = std::prev(current);
            if (previous->second.right == current->first
                && previous->second.value == current->second.value) {
                previous->second.right = current->second.right;
                intervals_.erase(current);
                current = previous;
            }
        }
        auto next = std::next(current);
        if (next != intervals_.end() && current->second.right == next->first
            && current->second.value == next->second.value) {
            current->second.right = next->second.right;
            intervals_.erase(next);
        }
        return removed;
    }

    /// O(log n)。coordinateを含む区間の値へのconst参照を返す。
    const Value& operator[](Coordinate coordinate) const {
        assert(domain_left_ <= coordinate && coordinate < domain_right_);
        auto iterator = intervals_.upper_bound(coordinate);
        assert(iterator != intervals_.begin());
        return std::prev(iterator)->second.value;
    }

    /// O(1)。現在の極大な定数区間数を返す。
    int segment_count() const noexcept {
        return static_cast<int>(intervals_.size());
    }

private:
    struct node {
        Coordinate right;
        Value value;
    };

    auto split(Coordinate coordinate) {
        if (coordinate == domain_right_) return intervals_.end();
        auto iterator = std::prev(intervals_.upper_bound(coordinate));
        if (iterator->first == coordinate) return iterator;
        assert(coordinate < iterator->second.right);
        const Coordinate right = iterator->second.right;
        const Value value = iterator->second.value;
        iterator->second.right = coordinate;
        return intervals_.emplace(coordinate, node{right, value}).first;
    }

    Coordinate domain_left_;
    Coordinate domain_right_;
    std::map<Coordinate, node> intervals_;
};

}
