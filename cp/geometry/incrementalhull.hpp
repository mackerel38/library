#pragma once
#include <bits/stdc++.h>
#include "geometry/point.hpp"

namespace poe {

/// 点を追加し、任意方向の内積最大値を管理する。add・max_dotは償却O(log^2 n)、領域O(n)。
template<class T>
struct incrementalconvexhull {
    using value_type = geometrywide<T>;

    /// O(1)。空集合を作る。
    incrementalconvexhull() = default;

    /// 償却O(log^2 n)。点を追加する。重複点も追加できる。
    void add(point<T> value) {
        std::vector<point<T>> carry{value};
        int level = 0;
        while (level < static_cast<int>(blocks_.size()) && blocks_[level]) {
            auto& points = blocks_[level]->points;
            carry.insert(carry.end(), points.begin(), points.end());
            blocks_[level].reset();
            ++level;
        }
        if (level == static_cast<int>(blocks_.size())) blocks_.push_back(std::nullopt);
        blocks_[level].emplace(std::move(carry));
        ++size_;
    }

    /// O(log^2 n)。追加済み点(x,y)に対するa*x+b*yの最大値を返す。空集合では呼べない。
    value_type max_dot(T a, T b) const {
        assert(size_ > 0);
        std::optional<value_type> answer;
        for (const auto& block : blocks_) {
            if (!block) continue;
            const value_type candidate = block->max_dot(a, b);
            if (!answer || candidate > *answer) answer = candidate;
        }
        return *answer;
    }

    /// O(1)。追加した点の個数を返す。重複も別々に数える。
    int size() const noexcept { return size_; }

    /// O(1)。点が一つもないならtrueを返す。
    bool empty() const noexcept { return size_ == 0; }

private:
    struct staticblock {
        std::vector<point<T>> points;
        std::vector<point<T>> lower;
        std::vector<point<T>> upper;

        explicit staticblock(std::vector<point<T>> source) : points(std::move(source)) {
            std::ranges::sort(points, {}, [](const point<T>& value) {
                return std::pair<T, T>{value.x, value.y};
            });
            std::vector<point<T>> low_points, high_points;
            for (int left = 0; left < static_cast<int>(points.size()); ) {
                int right = left + 1;
                while (right < static_cast<int>(points.size())
                       && points[right].x == points[left].x) ++right;
                low_points.push_back(points[left]);
                high_points.push_back(points[right - 1]);
                left = right;
            }
            for (const auto& current : low_points) {
                while (lower.size() >= 2
                       && cross(lower.back() - lower[lower.size() - 2],
                                current - lower.back()) <= 0) {
                    lower.pop_back();
                }
                lower.push_back(current);
            }
            for (const auto& current : high_points) {
                while (upper.size() >= 2
                       && cross(upper.back() - upper[upper.size() - 2],
                                current - upper.back()) >= 0) {
                    upper.pop_back();
                }
                upper.push_back(current);
            }
        }

        value_type max_dot(T a, T b) const {
            const auto& chain = b >= T{} ? upper : lower;
            const auto evaluate = [&](int index) -> value_type {
                return static_cast<value_type>(a) * chain[index].x
                     + static_cast<value_type>(b) * chain[index].y;
            };
            int left = 0, right = static_cast<int>(chain.size()) - 1;
            while (left < right) {
                const int middle = (left + right) / 2;
                if (evaluate(middle) <= evaluate(middle + 1)) left = middle + 1;
                else right = middle;
            }
            return evaluate(left);
        }
    };

    int size_ = 0;
    std::vector<std::optional<staticblock>> blocks_;
};

}  // namespace poe
