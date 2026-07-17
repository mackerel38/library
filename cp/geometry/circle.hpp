#pragma once
#include <bits/stdc++.h>
#include "geometry/point.hpp"

namespace poe {

/// circle{center,radius}: long doubleの円。
struct circle {
    point<long double> center{};
    long double radius = -1;

    /// O(1)。c.contains(p): 誤差を考慮してpが円内または円周上か返す。
    template<class T>
    bool contains(const point<T>& value) const {
        return radius >= 0 && distance(center, point<long double>{
            static_cast<long double>(value.x), static_cast<long double>(value.y)})
            <= radius + 1e-10L * std::max(1.0L, radius);
    }
};

namespace detail {

template<class T>
circle diameter_circle(const point<T>& first, const point<T>& second) {
    const point<long double> center{
        (static_cast<long double>(first.x) + second.x) / 2,
        (static_cast<long double>(first.y) + second.y) / 2
    };
    return {center, distance(first, second) / 2};
}

template<class T>
circle circumcircle(const point<T>& first, const point<T>& second, const point<T>& third) {
    const long double ax = first.x, ay = first.y;
    const long double bx = second.x, by = second.y;
    const long double cx = third.x, cy = third.y;
    const long double denominator = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
    if (std::abs(denominator) <= 1e-24L) {
        circle result = diameter_circle(first, second);
        for (const auto* left : {&first, &second, &third}) {
            for (const auto* right : {&first, &second, &third}) {
                circle candidate = diameter_circle(*left, *right);
                if (candidate.radius > result.radius && candidate.contains(first)
                    && candidate.contains(second) && candidate.contains(third)) result = candidate;
            }
        }
        return result;
    }
    const long double a2 = ax * ax + ay * ay;
    const long double b2 = bx * bx + by * by;
    const long double c2 = cx * cx + cy * cy;
    point<long double> center{
        (a2 * (by - cy) + b2 * (cy - ay) + c2 * (ay - by)) / denominator,
        (a2 * (cx - bx) + b2 * (ax - cx) + c2 * (bx - ax)) / denominator
    };
    return {center, distance(center, point<long double>{ax, ay})};
}

}

/// 期待O(n)。minimum_enclosing_circle(points): 全点を含む最小円を返す。空集合は半径0。
template<class T>
circle minimum_enclosing_circle(std::vector<point<T>> points) {
    if (points.empty()) return {{0, 0}, 0};
    std::mt19937 random(712367821);
    std::shuffle(points.begin(), points.end(), random);
    circle result{{static_cast<long double>(points[0].x), static_cast<long double>(points[0].y)}, 0};
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (result.contains(points[i])) continue;
        result = {{static_cast<long double>(points[i].x), static_cast<long double>(points[i].y)}, 0};
        for (int j = 0; j < i; ++j) {
            if (result.contains(points[j])) continue;
            result = detail::diameter_circle(points[i], points[j]);
            for (int k = 0; k < j; ++k) {
                if (!result.contains(points[k])) {
                    result = detail::circumcircle(points[i], points[j], points[k]);
                }
            }
        }
    }
    return result;
}

}
