#pragma once
#include <bits/stdc++.h>
#include "fps/polynomial.hpp"
#include "fps/transform.hpp"

namespace poe {

/// O(M(n)log^2 n)。順列pでp[i]>=threshold[i]をちょうどk個満たす個数を全k返す。
template <int mod>
std::vector<staticmodint<mod>> permutation_threshold_counts(
    std::vector<int> thresholds
) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    const int n = static_cast<int>(thresholds.size());
    assert(n < mod);
    for (int threshold : thresholds) assert(0 <= threshold && threshold <= n);
    std::ranges::sort(thresholds, std::greater{});

    std::vector<mint> roots(n);
    for (int index = 0; index < n; ++index) {
        const int constant = n - thresholds[index] - index;
        roots[index] = -mint{constant};
    }
    const series product = polynomial_from_roots<mod>(roots);
    std::vector<mint> points(static_cast<std::size_t>(n) + 1);
    for (int value = 0; value <= n; ++value) points[value] = value;
    const auto evaluated = multipoint_evaluation(product, points);

    std::vector<mint> factorial(static_cast<std::size_t>(n) + 1, 1);
    std::vector<mint> inverse_factorial(static_cast<std::size_t>(n) + 1, 1);
    for (int value = 1; value <= n; ++value) {
        factorial[value] = factorial[value - 1] * value;
    }
    inverse_factorial[n] = factorial[n].inv();
    for (int value = n; value > 0; --value) {
        inverse_factorial[value - 1] = inverse_factorial[value] * value;
    }
    series transformed(static_cast<std::size_t>(n) + 1);
    series exponential_negative(static_cast<std::size_t>(n) + 1);
    for (int degree = 0; degree <= n; ++degree) {
        transformed[degree] = evaluated[degree] * inverse_factorial[degree];
        exponential_negative[degree] =
            (degree % 2 == 0 ? mint{1} : mint{-1}) * inverse_factorial[degree];
    }
    const series dp = (transformed * exponential_negative).prefix(n + 1);
    series at_least_subsets(static_cast<std::size_t>(n) + 1);
    for (int count = 0; count <= n; ++count) {
        at_least_subsets[count] = dp[n - count] * factorial[n - count];
    }
    const series exact = upper_binomial_transform(at_least_subsets, mint{-1});
    return {exact.begin(), exact.end()};
}

}
