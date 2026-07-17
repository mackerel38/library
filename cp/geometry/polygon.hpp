#pragma once
#include <bits/stdc++.h>
#include "geometry/point.hpp"

namespace poe {

/// area2(polygon): 符号付き面積の2倍を返す。反時計回りなら正。O(n)。
template<class T>
geometrywide<T> area2(const std::vector<point<T>>& polygon) {
    geometrywide<T> result{};
    for (int i = 0; i < static_cast<int>(polygon.size()); ++i) {
        result += cross(polygon[i], polygon[(i + 1) % polygon.size()]);
    }
    return result;
}

/// O(n log n)。convex_hull(points,keep_collinear): 反時計回りの凸包を返す。重複点は除く。
template<class T>
std::vector<point<T>> convex_hull(std::vector<point<T>> points, bool keep_collinear = false) {
    std::ranges::sort(points);
    points.erase(std::unique(points.begin(), points.end()), points.end());
    if (points.size() <= 1) return points;
    std::vector<point<T>> lower, upper;
    auto append = [&](std::vector<point<T>>& hull, const point<T>& current) {
        while (hull.size() >= 2) {
            const int turn = orientation(hull[hull.size() - 2], hull.back(), current);
            if (turn > 0 || (keep_collinear && turn == 0)) break;
            hull.pop_back();
        }
        hull.push_back(current);
    };
    for (const auto& current : points) append(lower, current);
    for (auto it = points.rbegin(); it != points.rend(); ++it) append(upper, *it);
    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    if (keep_collinear && area2(lower) == geometrywide<T>{}) return points;
    return lower;
}

/// containment: 多角形に対する点の位置。
enum class containment { outside, boundary, inside };

/// O(n)。polygon_contains(polygon,p): 単純多角形に対するpの位置を返す。
template<class T>
containment polygon_contains(const std::vector<point<T>>& polygon, const point<T>& target) {
    bool inside = false;
    for (int i = 0; i < static_cast<int>(polygon.size()); ++i) {
        point<T> first = polygon[i];
        point<T> second = polygon[(i + 1) % polygon.size()];
        if (on_segment(first, second, target)) return containment::boundary;
        if (first.y > second.y) std::swap(first, second);
        if (first.y <= target.y && target.y < second.y && orientation(first, second, target) > 0) {
            inside = !inside;
        }
    }
    return inside ? containment::inside : containment::outside;
}

/// O(n)。is_convex(polygon,strict): 頂点順の多角形が凸か返す。strictなら一直線の辺を許さない。
template<class T>
bool is_convex(const std::vector<point<T>>& polygon, bool strict = true) {
    if (polygon.size() < 3) return false;
    int direction = 0;
    for (int i = 0; i < static_cast<int>(polygon.size()); ++i) {
        const int turn = orientation(
            polygon[i], polygon[(i + 1) % polygon.size()], polygon[(i + 2) % polygon.size()]);
        if (turn == 0) {
            if (strict) return false;
            continue;
        }
        if (direction != 0 && direction != turn) return false;
        direction = turn;
    }
    return direction != 0;
}

}
