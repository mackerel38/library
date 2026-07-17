#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 整数区間上の区分線形な上に凸列。点から作り、一次関数加算・level未満切除・suffix max変換を行う。
struct concavesequence {
    /// O(1)。座標xだけでvalueを取る列を作る。
    concavesequence(long long x, long long value) : points_{{x, value}} {}

    /// O(1)。定義域が空かを返す。
    bool empty() const noexcept { return points_.empty(); }

    /// O(1)。定義域の左端を返す。空でないこと。
    long long left() const { assert(!empty()); return points_.front().x; }

    /// O(1)。定義域の右端を返す。空でないこと。
    long long right() const { assert(!empty()); return points_.back().x; }

    /// O(log pieces)。xが定義域内なら値を返す。
    std::optional<long long> get(long long x) const {
        if (empty() || x < left() || right() < x) return std::nullopt;
        auto iterator = std::upper_bound(points_.begin(), points_.end(), x,
            [](long long coordinate, const point& item) { return coordinate < item.x; });
        if (iterator == points_.begin()) return evaluate(*iterator);
        --iterator;
        if (iterator->x == x || std::next(iterator) == points_.end()) return evaluate(*iterator);
        const auto next = std::next(iterator);
        return evaluate(*iterator) + total_slope(*iterator, *next) * (x - iterator->x);
    }

    /// O(1)。全xの値へslope*x+constantを加える。
    void add_affine(long long slope, long long constant) {
        lazy_slope_ += slope;
        lazy_constant_ += constant;
    }

    /// ならしO(pieces)。値がlevel未満の座標を定義域から除く。
    void trim_below(long long level) {
        while (!points_.empty() && evaluate(points_.back()) < level) {
            const point bad = points_.back();
            points_.pop_back();
            if (points_.empty()) break;
            const point good = points_.back();
            if (evaluate(good) < level) continue;
            const long long slope = total_slope(good, bad);
            assert(slope < 0);
            const long long distance = (evaluate(good) - level) / (-slope);
            if (distance > 0) points_.push_back(interpolate(good, bad, good.x + distance));
            break;
        }
        while (!points_.empty() && evaluate(points_.front()) < level) {
            const point bad = points_.front();
            points_.pop_front();
            if (points_.empty()) break;
            const point good = points_.front();
            if (evaluate(good) < level) continue;
            const long long slope = total_slope(bad, good);
            assert(slope > 0);
            const long long distance = (evaluate(good) - level) / slope;
            if (distance > 0) points_.push_front(interpolate(bad, good, good.x - distance));
            break;
        }
    }

    /// ならしO(pieces)。g(x)=max_{y>=x}(f(y)-slope*(y-x))へ変換し左端をnew_leftまで拡張する。
    void suffix_max(long long slope, long long new_left) {
        assert(!empty() && new_left <= left());
        while (points_.size() >= 2 && total_slope(points_[0], points_[1]) >= slope) {
            points_.pop_front();
        }
        if (new_left == left()) return;
        const point first = points_.front();
        const long long raw_slope = slope - lazy_slope_;
        points_.push_front({new_left, first.raw - raw_slope * (first.x - new_left)});
    }

private:
    struct point {
        long long x;
        long long raw;
    };

    long long evaluate(const point& item) const {
        return item.raw + lazy_slope_ * item.x + lazy_constant_;
    }

    long long raw_slope(const point& left, const point& right) const {
        assert(left.x < right.x);
        assert((right.raw - left.raw) % (right.x - left.x) == 0);
        return (right.raw - left.raw) / (right.x - left.x);
    }

    long long total_slope(const point& left, const point& right) const {
        return raw_slope(left, right) + lazy_slope_;
    }

    point interpolate(const point& left, const point& right, long long x) const {
        assert(left.x <= x && x <= right.x);
        return {x, left.raw + raw_slope(left, right) * (x - left.x)};
    }

    std::deque<point> points_;
    long long lazy_slope_ = 0;
    long long lazy_constant_ = 0;
};

}
