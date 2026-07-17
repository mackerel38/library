#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 半開軸平行長方形[left,right)x[bottom,top)を表す。
template <class T>
struct axisalignedrectangle {
    T left;
    T right;
    T bottom;
    T top;
};

/// O(1)。半開長方形の面積を返す。
template <class T>
T rectangle_area(const axisalignedrectangle<T>& rectangle) {
    assert(rectangle.left <= rectangle.right && rectangle.bottom <= rectangle.top);
    return (rectangle.right - rectangle.left) * (rectangle.top - rectangle.bottom);
}

/// O(1)。半開長方形をx=coordinateで空でない高々二つへ分割する。
template <class T>
std::vector<axisalignedrectangle<T>> split_rectangle_x(
    const axisalignedrectangle<T>& rectangle,
    const T& coordinate
) {
    assert(rectangle.left <= rectangle.right && rectangle.bottom <= rectangle.top);
    if (coordinate <= rectangle.left || rectangle.right <= coordinate) return {rectangle};
    return {
        {rectangle.left, coordinate, rectangle.bottom, rectangle.top},
        {coordinate, rectangle.right, rectangle.bottom, rectangle.top}
    };
}

/// O(1)。半開長方形をy=coordinateで空でない高々二つへ分割する。
template <class T>
std::vector<axisalignedrectangle<T>> split_rectangle_y(
    const axisalignedrectangle<T>& rectangle,
    const T& coordinate
) {
    assert(rectangle.left <= rectangle.right && rectangle.bottom <= rectangle.top);
    if (coordinate <= rectangle.bottom || rectangle.top <= coordinate) return {rectangle};
    return {
        {rectangle.left, rectangle.right, rectangle.bottom, coordinate},
        {rectangle.left, rectangle.right, coordinate, rectangle.top}
    };
}

/// O(1)。半開長方形を(dx,dy)だけ平行移動する。
template <class T>
axisalignedrectangle<T> translate_rectangle(
    axisalignedrectangle<T> rectangle,
    const T& dx,
    const T& dy
) {
    rectangle.left += dx;
    rectangle.right += dx;
    rectangle.bottom += dy;
    rectangle.top += dy;
    return rectangle;
}

/// O(1)。面積正の半開長方形が正の長さの辺で接するならtrueを返す。点接触・重なりはfalse。
template <class T>
bool rectangles_edge_adjacent(
    const axisalignedrectangle<T>& left,
    const axisalignedrectangle<T>& right
) {
    assert(left.left < left.right && left.bottom < left.top);
    assert(right.left < right.right && right.bottom < right.top);
    const bool vertical_overlap = std::max(left.bottom, right.bottom) < std::min(left.top, right.top);
    const bool horizontal_overlap = std::max(left.left, right.left) < std::min(left.right, right.right);
    return ((left.right == right.left || right.right == left.left) && vertical_overlap)
        || ((left.top == right.bottom || right.top == left.bottom) && horizontal_overlap);
}

/// O(n log n)。半開軸平行長方形群の和集合面積を返す。
template <std::integral T>
T rectangle_union_area(const std::vector<axisalignedrectangle<T>>& rectangles) {
    struct event {
        T x;
        T bottom;
        T top;
        int delta;
    };
    std::vector<T> ys;
    std::vector<event> events;
    for (const auto& rectangle : rectangles) {
        assert(rectangle.left <= rectangle.right && rectangle.bottom <= rectangle.top);
        if (rectangle.left == rectangle.right || rectangle.bottom == rectangle.top) continue;
        ys.push_back(rectangle.bottom);
        ys.push_back(rectangle.top);
        events.push_back({rectangle.left, rectangle.bottom, rectangle.top, 1});
        events.push_back({rectangle.right, rectangle.bottom, rectangle.top, -1});
    }
    if (events.empty()) return T{};
    std::ranges::sort(ys);
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
    std::ranges::sort(events, {}, &event::x);
    const int segments = static_cast<int>(ys.size()) - 1;
    std::vector<int> cover(4 * std::max(1, segments));
    std::vector<T> covered(cover.size());
    auto update = [&](auto&& self, int node, int left, int right,
                      int query_left, int query_right, int delta) -> void {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            cover[node] += delta;
        } else {
            const int middle = (left + right) / 2;
            self(self, node * 2, left, middle, query_left, query_right, delta);
            self(self, node * 2 + 1, middle, right, query_left, query_right, delta);
        }
        if (cover[node] > 0) covered[node] = ys[right] - ys[left];
        else if (right - left == 1) covered[node] = T{};
        else covered[node] = covered[node * 2] + covered[node * 2 + 1];
    };

    T area{};
    T previous_x = events.front().x;
    for (int index = 0; index < static_cast<int>(events.size());) {
        const T x = events[index].x;
        area += (x - previous_x) * covered[1];
        while (index < static_cast<int>(events.size()) && events[index].x == x) {
            const int bottom = std::lower_bound(ys.begin(), ys.end(), events[index].bottom) - ys.begin();
            const int top = std::lower_bound(ys.begin(), ys.end(), events[index].top) - ys.begin();
            update(update, 1, 0, segments, bottom, top, events[index].delta);
            ++index;
        }
        previous_x = x;
    }
    return area;
}

}
