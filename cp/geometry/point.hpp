#pragma once
#include <bits/stdc++.h>

namespace poe {

template<class T>
using geometrywide = std::conditional_t<std::is_integral_v<T>, __int128_t, long double>;

/// point<T>{x,y}: 2次元点・ベクトル。+,-,scalar積と辞書順比較が使える。
template<class T>
struct point {
    T x{};
    T y{};

    point& operator+=(const point& other) { x += other.x; y += other.y; return *this; }
    point& operator-=(const point& other) { x -= other.x; y -= other.y; return *this; }
    point& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
    point& operator/=(T scalar) { x /= scalar; y /= scalar; return *this; }

    friend point operator+(point left, const point& right) { return left += right; }
    friend point operator-(point left, const point& right) { return left -= right; }
    friend point operator*(point value, T scalar) { return value *= scalar; }
    friend point operator*(T scalar, point value) { return value *= scalar; }
    friend point operator/(point value, T scalar) { return value /= scalar; }
    friend bool operator==(const point&, const point&) = default;
    friend auto operator<=>(const point&, const point&) = default;
};

/// O(1)。dot(a,b): ベクトルa,bの内積を返す。整数型では__int128_tへ拡張する。
template<class T>
geometrywide<T> dot(const point<T>& left, const point<T>& right) {
    return geometrywide<T>{left.x} * right.x + geometrywide<T>{left.y} * right.y;
}

/// O(1)。cross(a,b): ベクトルa,bの外積を返す。整数型では__int128_tへ拡張する。
template<class T>
geometrywide<T> cross(const point<T>& left, const point<T>& right) {
    return geometrywide<T>{left.x} * right.y - geometrywide<T>{left.y} * right.x;
}

/// O(1)。cross(a,b,c): (b-a)x(c-a)を返す。
template<class T>
geometrywide<T> cross(const point<T>& first, const point<T>& second, const point<T>& third) {
    return cross(second - first, third - first);
}

/// O(1)。orientation(a,b,c): 反時計回りなら1、時計回りなら-1、一直線なら0。
template<class T>
int orientation(const point<T>& first, const point<T>& second, const point<T>& third) {
    const auto value = cross(first, second, third);
    if constexpr (std::is_integral_v<T>) {
        return (value > 0) - (value < 0);
    } else {
        const long double scale = std::max({1.0L, std::abs(static_cast<long double>(value))});
        if (std::abs(static_cast<long double>(value)) <= 1e-12L * scale) return 0;
        return value > 0 ? 1 : -1;
    }
}

/// O(1)。on_segment(a,b,p): 線分ab上にpがあるか返す。
template<class T>
bool on_segment(const point<T>& first, const point<T>& second, const point<T>& target) {
    if (orientation(first, second, target) != 0) return false;
    return dot(target - first, target - second) <= 0;
}

/// O(1)。segments_intersect(a,b,c,d): 端点接触を含め、二線分が交わるか返す。
template<class T>
bool segments_intersect(
    const point<T>& first,
    const point<T>& second,
    const point<T>& third,
    const point<T>& fourth
) {
    const int a = orientation(first, second, third);
    const int b = orientation(first, second, fourth);
    const int c = orientation(third, fourth, first);
    const int d = orientation(third, fourth, second);
    if (a == 0 && on_segment(first, second, third)) return true;
    if (b == 0 && on_segment(first, second, fourth)) return true;
    if (c == 0 && on_segment(third, fourth, first)) return true;
    if (d == 0 && on_segment(third, fourth, second)) return true;
    return a * b < 0 && c * d < 0;
}

/// O(1)。distance(a,b): 二点間のEuclid距離をlong doubleで返す。
template<class T>
long double distance(const point<T>& first, const point<T>& second) {
    const long double dx = static_cast<long double>(first.x) - second.x;
    const long double dy = static_cast<long double>(first.y) - second.y;
    return std::hypotl(dx, dy);
}

/// O(1)。distance_to_segment(p,a,b): 点pと線分abの最短距離を返す。
template<class T>
long double distance_to_segment(
    const point<T>& target,
    const point<T>& first,
    const point<T>& second
) {
    const long double dx = static_cast<long double>(second.x) - first.x;
    const long double dy = static_cast<long double>(second.y) - first.y;
    const long double length_square = dx * dx + dy * dy;
    if (length_square == 0) return distance(target, first);
    const long double tx = static_cast<long double>(target.x) - first.x;
    const long double ty = static_cast<long double>(target.y) - first.y;
    const long double ratio = std::clamp(
        (tx * dx + ty * dy) / length_square, 0.0L, 1.0L);
    const long double nearest_x = static_cast<long double>(first.x) + ratio * dx;
    const long double nearest_y = static_cast<long double>(first.y) + ratio * dy;
    return std::hypotl(static_cast<long double>(target.x) - nearest_x,
                       static_cast<long double>(target.y) - nearest_y);
}

/// O(1)。二点が各線分を速さ1で同時に進み、到着後停止するときの最接近距離を返す。
template<class T>
long double minimum_distance_segment_walks(
    const point<T>& first_start,
    const point<T>& first_goal,
    const point<T>& second_start,
    const point<T>& second_goal
) {
    const auto convert = [](const point<T>& value) {
        return point<long double>{static_cast<long double>(value.x),
                                  static_cast<long double>(value.y)};
    };
    const point<long double> first_begin = convert(first_start);
    const point<long double> first_end = convert(first_goal);
    const point<long double> second_begin = convert(second_start);
    const point<long double> second_end = convert(second_goal);
    const long double first_time = distance(first_begin, first_end);
    const long double second_time = distance(second_begin, second_end);
    assert(first_time > 0 && second_time > 0);

    const point<long double> first_velocity =
        (first_end - first_begin) / first_time;
    const point<long double> second_velocity =
        (second_end - second_begin) / second_time;
    const long double middle_time = std::min(first_time, second_time);
    const point<long double> relative_start = first_begin - second_begin;
    const point<long double> relative_middle =
        first_begin + first_velocity * middle_time
        - second_begin - second_velocity * middle_time;
    const point<long double> relative_end = first_end - second_end;
    const point<long double> origin{};
    return std::min(
        distance_to_segment(origin, relative_start, relative_middle),
        distance_to_segment(origin, relative_middle, relative_end));
}

/// O(1)。偏角を[0,pi)と[pi,2pi)へ分ける。原点ベクトルには使わない。
template<class T>
int argument_half(const point<T>& value) {
    assert(value.x != T{} || value.y != T{});
    return value.y < T{} || (value.y == T{} && value.x < T{});
}

/// O(1)。偏角の昇順比較を返す。同じ方向は同値として扱う。
template<class T>
bool argument_less(const point<T>& left, const point<T>& right) {
    const int left_half = argument_half(left);
    const int right_half = argument_half(right);
    if (left_half != right_half) return left_half < right_half;
    return cross(left, right) > 0;
}

/// O(1)。原点から見て同じ向きのray上にあるか返す。
template<class T>
bool same_direction(const point<T>& left, const point<T>& right) {
    return cross(left, right) == 0 && dot(left, right) > 0;
}

}
