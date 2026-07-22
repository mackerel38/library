#pragma once
#include <bits/stdc++.h>
#include "geometry/polygon.hpp"

namespace poe {

/// O(n log n)。反時計回りの凸多角形の内部を避けてstartからgoalへ進む最短距離を返す。
template<class T>
long double shortest_path_avoiding_convex_polygon(
    const std::vector<point<T>>& polygon,
    const point<T>& start,
    const point<T>& goal
) {
    assert(polygon.size() >= 3);
    assert(is_convex(polygon, false));
    assert(area2(polygon) > geometrywide<T>{});
    assert(polygon_contains(polygon, start) != containment::inside);
    assert(polygon_contains(polygon, goal) != containment::inside);
    if (start == goal) return 0;

    std::vector<point<T>> points = polygon;
    points.push_back(start);
    points.push_back(goal);
    const auto hull = convex_hull(std::move(points), true);
    const auto start_it = std::ranges::find(hull, start);
    const auto goal_it = std::ranges::find(hull, goal);
    if (start_it == hull.end() || goal_it == hull.end()) {
        return distance(start, goal);
    }

    const int n = static_cast<int>(hull.size());
    const int start_index = static_cast<int>(start_it - hull.begin());
    const int goal_index = static_cast<int>(goal_it - hull.begin());
    std::vector<long double> prefix(n + 1);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + distance(hull[i], hull[(i + 1) % n]);
    }
    const int left = std::min(start_index, goal_index);
    const int right = std::max(start_index, goal_index);
    const long double one_way = prefix[right] - prefix[left];
    return std::min(one_way, prefix[n] - one_way);
}

}
