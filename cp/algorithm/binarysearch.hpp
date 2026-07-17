#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(log(right-left))。falseからtrueへ変わる[left,right)内の最初を返し、なければnullopt。
template<std::integral T, class Predicate>
std::optional<T> first_true(T left, T right, Predicate predicate) {
    assert(left <= right);
    const T end = right;
    while (left < right) {
        const T middle = std::midpoint(left, right);
        if (predicate(middle)) right = middle;
        else left = middle + 1;
    }
    if (left == end) return std::nullopt;
    return left;
}

/// O(log(right-left))。trueからfalseへ変わる[left,right)内の最後を返し、なければnullopt。
template<std::integral T, class Predicate>
std::optional<T> last_true(T left, T right, Predicate predicate) {
    assert(left <= right);
    const T begin = left;
    while (left < right) {
        const T middle = std::midpoint(left, right);
        if (predicate(middle)) left = middle + 1;
        else right = middle;
    }
    if (left == begin) return std::nullopt;
    return left - 1;
}

/// O(iterations)。falseからtrueへ変わる実数述語を二分探索し、true側の近似境界を返す。
template<std::floating_point T, class Predicate>
T binary_search_real(T false_value, T true_value, Predicate predicate, int iterations = 100) {
    assert(false_value <= true_value && iterations >= 0);
    for (int iteration = 0; iteration < iterations; ++iteration) {
        const T middle = std::midpoint(false_value, true_value);
        if (predicate(middle)) true_value = middle;
        else false_value = middle;
    }
    return true_value;
}

/// minimize_convex_realの近似結果。argumentでvalueを取る。
template<std::floating_point T>
struct realminimum {
    T argument;
    T value;
};

/// O(iterations)。[left,right]上の凸関数を三分探索し、最小点と最小値の近似を返す。
template<std::floating_point T, class Function>
realminimum<T> minimize_convex_real(
    T left,
    T right,
    Function function,
    int iterations = 100
) {
    assert(left <= right && iterations >= 0);
    for (int iteration = 0; iteration < iterations; ++iteration) {
        const T first = (left * 2 + right) / 3;
        const T second = (left + right * 2) / 3;
        if (function(first) < function(second)) right = second;
        else left = first;
    }
    const T argument = std::midpoint(left, right);
    return {argument, function(argument)};
}

}
