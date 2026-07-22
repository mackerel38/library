#pragma once
#include <bits/stdc++.h>
#include "geometry/point.hpp"

namespace poe {

namespace detail {

using latticewide = __int128_t;

inline latticewide lattice_floor_div(latticewide numerator, latticewide denominator) {
    assert(denominator != 0);
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }
    latticewide quotient = numerator / denominator;
    if (numerator % denominator < 0) --quotient;
    return quotient;
}

inline latticewide lattice_ceil_div(latticewide numerator, latticewide denominator) {
    return -lattice_floor_div(-numerator, denominator);
}

inline std::tuple<latticewide, latticewide, latticewide> lattice_extended_gcd(
    latticewide first,
    latticewide second
) {
    latticewide old_r = first;
    latticewide r = second;
    latticewide old_x = 1;
    latticewide x = 0;
    latticewide old_y = 0;
    latticewide y = 1;
    while (r != 0) {
        const latticewide quotient = old_r / r;
        std::tie(old_r, r) = std::pair{r, old_r - quotient * r};
        std::tie(old_x, x) = std::pair{x, old_x - quotient * x};
        std::tie(old_y, y) = std::pair{y, old_y - quotient * y};
    }
    return {old_r, old_x, old_y};
}

inline std::optional<long long> one_lattice_move(
    const point<long long>& target,
    const point<long long>& step
) {
    if (target == point<long long>{}) return 0;
    if (step == point<long long>{}) return std::nullopt;
    const latticewide divisor = step.x != 0 ? step.x : step.y;
    const latticewide value = step.x != 0 ? target.x : target.y;
    if (value % divisor != 0) return std::nullopt;
    const latticewide count = value / divisor;
    if (count < 0 || count > std::numeric_limits<long long>::max()) return std::nullopt;
    if (latticewide{step.x} * count != target.x ||
        latticewide{step.y} * count != target.y) return std::nullopt;
    return static_cast<long long>(count);
}

inline std::optional<long long> collinear_lattice_moves(
    const point<long long>& target,
    const point<long long>& first,
    const point<long long>& second
) {
    if (first == point<long long>{}) return one_lattice_move(target, second);
    if (second == point<long long>{}) return one_lattice_move(target, first);
    const point<long long> direction = first;
    if (latticewide{direction.x} * target.y != latticewide{direction.y} * target.x) {
        return std::nullopt;
    }
    const latticewide p = direction.x != 0 ? first.x : first.y;
    const latticewide q = direction.x != 0 ? second.x : second.y;
    const latticewide r = direction.x != 0 ? target.x : target.y;
    const latticewide abs_p = p < 0 ? -p : p;
    const latticewide abs_q = q < 0 ? -q : q;
    const auto [gcd, bezout_p, bezout_q] = lattice_extended_gcd(abs_p, abs_q);
    if (r % gcd != 0) return std::nullopt;

    latticewide first_count = bezout_p * (r / gcd) * (p < 0 ? -1 : 1);
    latticewide second_count = bezout_q * (r / gcd) * (q < 0 ? -1 : 1);
    const latticewide first_step = q / gcd;
    const latticewide second_step = -p / gcd;
    constexpr latticewide infinity = latticewide{1} << 120;
    latticewide lower = -infinity;
    latticewide upper = infinity;
    const auto restrict_nonnegative = [&](latticewide value, latticewide step) {
        if (step > 0) lower = std::max(lower, lattice_ceil_div(-value, step));
        else if (step < 0) upper = std::min(upper, lattice_floor_div(value, -step));
        else if (value < 0) lower = 1, upper = 0;
    };
    restrict_nonnegative(first_count, first_step);
    restrict_nonnegative(second_count, second_step);
    if (lower > upper) return std::nullopt;

    const latticewide slope = first_step + second_step;
    latticewide parameter = 0;
    if (slope > 0) parameter = lower;
    else if (slope < 0) parameter = upper;
    else parameter = std::clamp<latticewide>(0, lower, upper);
    first_count += first_step * parameter;
    second_count += second_step * parameter;
    const latticewide answer = first_count + second_count;
    if (first_count < 0 || second_count < 0 || answer > std::numeric_limits<long long>::max()) {
        return std::nullopt;
    }
    return static_cast<long long>(answer);
}

}

/// O(1)。target=x*first+y*second、x,y>=0を満たす最小x+yを返し、存在しなければnullopt。
inline std::optional<long long> minimum_lattice_moves(
    const point<long long>& target,
    const point<long long>& first,
    const point<long long>& second
) {
    if (target == point<long long>{}) return 0;
    const detail::latticewide determinant =
        detail::latticewide{first.x} * second.y - detail::latticewide{first.y} * second.x;
    if (determinant == 0) {
        return detail::collinear_lattice_moves(target, first, second);
    }
    const detail::latticewide first_numerator =
        detail::latticewide{target.x} * second.y - detail::latticewide{target.y} * second.x;
    const detail::latticewide second_numerator =
        detail::latticewide{first.x} * target.y - detail::latticewide{first.y} * target.x;
    if (first_numerator % determinant != 0 || second_numerator % determinant != 0) {
        return std::nullopt;
    }
    const detail::latticewide first_count = first_numerator / determinant;
    const detail::latticewide second_count = second_numerator / determinant;
    const detail::latticewide answer = first_count + second_count;
    if (first_count < 0 || second_count < 0 || answer > std::numeric_limits<long long>::max()) {
        return std::nullopt;
    }
    return static_cast<long long>(answer);
}

/// O(k^2)。stepsのうち高々2種類を反復してtargetへ到達する最小手数を返す。
inline std::optional<long long> minimum_two_kind_lattice_moves(
    const point<long long>& target,
    std::span<const point<long long>> steps
) {
    if (target == point<long long>{}) return 0;
    std::optional<long long> answer;
    const auto relax = [&](std::optional<long long> candidate) {
        if (candidate && (!answer || *candidate < *answer)) answer = candidate;
    };
    for (std::size_t first = 0; first < steps.size(); ++first) {
        relax(detail::one_lattice_move(target, steps[first]));
        for (std::size_t second = first + 1; second < steps.size(); ++second) {
            relax(minimum_lattice_moves(target, steps[first], steps[second]));
        }
    }
    return answer;
}

}
