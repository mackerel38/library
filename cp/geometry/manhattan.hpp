#pragma once
#include <bits/stdc++.h>
#include "structure/pointcount2d.hpp"

namespace poe {

/// 静的点集合のManhattan近傍: manhattannearest<long long> near(points); kth(x,y,k)はO(log C log n)。
template <std::signed_integral T = long long>
struct manhattannearest {
    using point = std::pair<T, T>;

    /// O(n log n)。重複を含め、pointsの各点を一個として構築する。
    explicit manhattannearest(const std::vector<point>& points)
        : points_(points), transformed_(transform(points)) {
        if (points.empty()) return;
        minimum_sum_ = maximum_sum_ = points[0].first + points[0].second;
        minimum_difference_ = maximum_difference_ = points[0].first - points[0].second;
        for (const auto& [x, y] : points) {
            minimum_sum_ = std::min(minimum_sum_, x + y);
            maximum_sum_ = std::max(maximum_sum_, x + y);
            minimum_difference_ = std::min(minimum_difference_, x - y);
            maximum_difference_ = std::max(maximum_difference_, x - y);
        }
    }

    /// O(1)。点の個数を返す。
    int size() const noexcept { return static_cast<int>(points_.size()); }

    /// O(1)。入力順でindex番目の点を返す。
    const point& operator[](int index) const {
        assert(0 <= index && index < size());
        return points_[index];
    }

    /// O(log n)。(x,y)からManhattan距離distance以下にある点数を返す。
    int count(T x, T y, T distance) const {
        assert(distance >= 0);
        const T sum = x + y;
        const T difference = x - y;
        return transformed_.count_closed(
            sum - distance,
            difference - distance,
            sum + distance,
            difference + distance
        );
    }

    /// O(log C log n)。(x,y)から1-indexedでk番目に近い点までのManhattan距離を返す。
    T kth(T x, T y, int k) const {
        assert(1 <= k && k <= size());
        const T sum = x + y;
        const T difference = x - y;
        T low = -1;
        T high = std::max({
            std::abs(sum - minimum_sum_),
            std::abs(sum - maximum_sum_),
            std::abs(difference - minimum_difference_),
            std::abs(difference - maximum_difference_)
        });
        while (high - low > 1) {
            const T middle = low + (high - low) / 2;
            if (count(x, y, middle) >= k) high = middle;
            else low = middle;
        }
        return high;
    }

private:
    static std::vector<point> transform(const std::vector<point>& points) {
        std::vector<point> result;
        result.reserve(points.size());
        for (const auto& [x, y] : points) result.emplace_back(x + y, x - y);
        return result;
    }

    std::vector<point> points_;
    pointcount2d<T> transformed_;
    T minimum_sum_{};
    T maximum_sum_{};
    T minimum_difference_{};
    T maximum_difference_{};
};

}
