#pragma once
#include <bits/stdc++.h>
#include "algorithm/intervalcrossing.hpp"
#include "geometry/point.hpp"

namespace poe {

/// line<T>{a,b,c}: ax+by+c=0で表す直線。a,bの少なくとも一方は非zero。
template <class T>
struct line {
    T a{};
    T b{};
    T c{};
};

/// O(1)。平行でない二直線の交点を返す。平行ならnullopt。
template <class T, class U>
std::optional<point<long double>> line_intersection(
    const line<T>& first,
    const line<U>& second
) {
    const long double determinant =
        static_cast<long double>(first.a) * second.b
        - static_cast<long double>(first.b) * second.a;
    const long double scale = std::max({
        1.0L,
        std::abs(static_cast<long double>(first.a) * second.b),
        std::abs(static_cast<long double>(first.b) * second.a)
    });
    if (std::abs(determinant) <= 1e-24L * scale) return std::nullopt;
    return point<long double>{
        (static_cast<long double>(first.b) * second.c
         - static_cast<long double>(first.c) * second.b) / determinant,
        (static_cast<long double>(first.c) * second.a
         - static_cast<long double>(first.a) * second.c) / determinant
    };
}

/// O(1)。点targetと直線valueの距離を返す。
template <class T, class U>
long double distance_to_line(const point<T>& target, const line<U>& value) {
    const long double numerator = std::abs(
        static_cast<long double>(value.a) * target.x
        + static_cast<long double>(value.b) * target.y + value.c
    );
    const long double denominator = std::hypotl(
        static_cast<long double>(value.a), static_cast<long double>(value.b)
    );
    assert(denominator > 0);
    return numerator / denominator;
}

/// O(n log n)時間・O(n)領域。二直線の交点のうちopen円内にある組数を返す。
template <class T>
long long count_line_intersections_in_circle(
    const std::vector<line<T>>& lines,
    const point<long double>& center,
    long double radius
) {
    assert(radius >= 0);
    std::vector<std::pair<long double, long double>> chords;
    chords.reserve(lines.size());
    constexpr long double pi = std::numbers::pi_v<long double>;
    for (const auto& value : lines) {
        const long double a = value.a;
        const long double b = value.b;
        const long double shifted_c = a * center.x + b * center.y + value.c;
        const long double norm_square = a * a + b * b;
        assert(norm_square > 0);
        const long double distance_square = shifted_c * shifted_c / norm_square;
        if (!(distance_square < radius * radius)) continue;
        const long double factor = -shifted_c / norm_square;
        const long double foot_x = a * factor;
        const long double foot_y = b * factor;
        const long double offset = std::sqrt(std::max(0.0L, radius * radius - distance_square));
        const long double inverse_norm = 1 / std::sqrt(norm_square);
        const long double direction_x = -b * inverse_norm;
        const long double direction_y = a * inverse_norm;
        auto angle = [&](long double sign) {
            long double result = std::atan2(
                foot_y + sign * direction_y * offset,
                foot_x + sign * direction_x * offset
            );
            if (result < 0) result += 2 * pi;
            return result;
        };
        long double first = angle(-1), second = angle(1);
        if (second < first) std::swap(first, second);
        chords.emplace_back(first, second);
    }
    if (chords.size() < 2) return 0;
    return count_alternating_interval_pairs(chords);
}

/// O(iterations n log n)時間。原点からk番目に近い二直線交点の距離を返す。
/// 全直線は互いに非平行、1<=k<=n(n-1)/2であること。
template <class T>
long double kth_line_intersection_distance(
    const std::vector<line<T>>& lines,
    long long k,
    int iterations = 50,
    long double initial_high = 1
) {
    const long long pairs = static_cast<long long>(lines.size()) * (lines.size() - 1) / 2;
    assert(1 <= k && k <= pairs && iterations > 0 && initial_high > 0);
    long double low = 0;
    long double high = initial_high;
    const point<long double> origin{};
    while (count_line_intersections_in_circle(lines, origin, high) < k) high *= 2;
    for (int iteration = 0; iteration < iterations; ++iteration) {
        const long double middle = std::midpoint(low, high);
        if (count_line_intersections_in_circle(lines, origin, middle) >= k) high = middle;
        else low = middle;
    }
    return high;
}

}
