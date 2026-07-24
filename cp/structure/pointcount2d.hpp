#pragma once
#include <bits/stdc++.h>
#include "structure/waveletmatrix.hpp"

namespace poe {

/// 静的点集合の長方形内点数: pointcount2d<long long> points(data); 構築O(n log n)、取得O(log n)。
template <class T>
struct pointcount2d {
    using point = std::pair<T, T>;

    /// O(n log n)。重複を含め、pointsの各点を一個として構築する。
    explicit pointcount2d(const std::vector<point>& points)
        : points_(points), sorted_(points) {
        std::sort(sorted_.begin(), sorted_.end());
        std::vector<T> y;
        y.reserve(sorted_.size());
        x_.reserve(sorted_.size());
        for (const auto& [x, value_y] : sorted_) {
            x_.push_back(x);
            y.push_back(value_y);
        }
        y_.emplace(y);
    }

    /// O(1)。点の個数を返す。
    int size() const noexcept { return static_cast<int>(points_.size()); }

    /// O(1)。入力順でindex番目の点を返す。
    const point& operator[](int index) const {
        assert(0 <= index && index < size());
        return points_[index];
    }

    /// O(log n)。[x_lower,x_upper)x[y_lower,y_upper)にある点数を返す。
    int count(const T& x_lower, const T& y_lower, const T& x_upper, const T& y_upper) const {
        assert(!(x_upper < x_lower) && !(y_upper < y_lower));
        const int left = static_cast<int>(std::lower_bound(x_.begin(), x_.end(), x_lower) - x_.begin());
        const int right = static_cast<int>(std::lower_bound(x_.begin(), x_.end(), x_upper) - x_.begin());
        return y_->count(left, right, y_lower, y_upper);
    }

    /// O(log n)。[x_lower,x_upper]x[y_lower,y_upper]にある点数を返す。
    int count_closed(
        const T& x_lower,
        const T& y_lower,
        const T& x_upper,
        const T& y_upper
    ) const {
        assert(!(x_upper < x_lower) && !(y_upper < y_lower));
        const int left = static_cast<int>(std::lower_bound(x_.begin(), x_.end(), x_lower) - x_.begin());
        const int right = static_cast<int>(std::upper_bound(x_.begin(), x_.end(), x_upper) - x_.begin());
        return y_->count_leq(left, right, y_upper) - y_->count_less(left, right, y_lower);
    }

private:
    std::vector<point> points_;
    std::vector<point> sorted_;
    std::vector<T> x_;
    std::optional<waveletmatrix<T>> y_;
};

}
