#pragma once
#include <bits/stdc++.h>
#include "fps/fps.hpp"

namespace poe {
namespace detail {

template <int mod>
std::vector<fps<mod>> bivariate_product(
    const std::vector<fps<mod>>& left,
    const std::vector<fps<mod>>& right,
    int y_size
) {
    using mint = staticmodint<mod>;
    assert(y_size > 0);
    if (left.empty() || right.empty()) return {};
    std::vector<mint> flattened_left(left.size() * static_cast<std::size_t>(y_size));
    std::vector<mint> flattened_right(right.size() * static_cast<std::size_t>(y_size));
    for (int x = 0; x < static_cast<int>(left.size()); ++x) {
        for (int y = 0; y < std::min(y_size, left[x].size_int()); ++y) {
            flattened_left[static_cast<std::size_t>(x) * y_size + y] = left[x][y];
        }
    }
    for (int x = 0; x < static_cast<int>(right.size()); ++x) {
        for (int y = 0; y < std::min(y_size, right[x].size_int()); ++y) {
            flattened_right[static_cast<std::size_t>(x) * y_size + y] = right[x][y];
        }
    }
    const auto product = convolution(std::move(flattened_left), std::move(flattened_right));
    std::vector<fps<mod>> result(left.size() + right.size() - 1, fps<mod>(y_size));
    for (int index = 0; index < static_cast<int>(product.size()); ++index) {
        const int x = index / y_size;
        const int y = index % y_size;
        if (x < static_cast<int>(result.size())) result[x][y] += product[index];
    }
    return result;
}

template <int mod>
fps<mod> power_projection_zero_constant(fps<mod> function, const fps<mod>& multiplier) {
    using series = fps<mod>;
    const int original_size = function.size_int();
    int degree = original_size - 1;
    int y_degree = 1;
    std::vector<series> numerator(degree + 1, series(2));
    std::vector<series> denominator(degree + 1, series(2));
    denominator[0][0] = 1;
    for (int x = 0; x <= degree; ++x) {
        numerator[x][0] = multiplier[x];
        denominator[x][1] = -function[x];
    }
    while (degree > 0) {
        auto negative_x = denominator;
        for (int x = 1; x <= degree; x += 2) {
            for (auto& coefficient : negative_x[x]) coefficient = -coefficient;
        }
        const int next_y_size = 2 * y_degree + 1;
        auto product_p = bivariate_product(numerator, negative_x, next_y_size);
        auto product_q = bivariate_product(denominator, negative_x, next_y_size);
        std::vector<series> next_p(degree / 2 + 1, series(next_y_size));
        std::vector<series> next_q(degree / 2 + 1, series(next_y_size));
        for (int x = 0; x <= degree / 2; ++x) {
            next_p[x] = std::move(product_p[2 * x + degree % 2]);
            next_q[x] = std::move(product_q[2 * x]);
        }
        numerator = std::move(next_p);
        denominator = std::move(next_q);
        degree /= 2;
        y_degree *= 2;
    }
    return numerator[0].div(denominator[0], original_size);
}

}

/// O(n log^2 n)。result[i]=[x^n]function(x)^i*multiplier(x)をi=0..nで返す。
template <int mod>
fps<mod> power_projection(fps<mod> function, const fps<mod>& multiplier) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    assert(!function.empty() && function.size() == multiplier.size());
    const int size = function.size_int();
    const mint constant = function[0];
    function[0] = 0;
    series projected = detail::power_projection_zero_constant(function, multiplier);
    if (constant == mint{}) return projected;

    std::vector<mint> factorial(size), inverse_factorial(size);
    factorial[0] = 1;
    for (int i = 1; i < size; ++i) factorial[i] = factorial[i - 1] * i;
    inverse_factorial.back() = factorial.back().inv();
    for (int i = size - 1; i >= 1; --i) inverse_factorial[i - 1] = inverse_factorial[i] * i;
    std::vector<mint> left(size), right(size);
    mint power = 1;
    for (int i = 0; i < size; ++i) {
        left[i] = projected[i] * inverse_factorial[i];
        right[i] = power * inverse_factorial[i];
        power *= constant;
    }
    const auto transformed = convolution(std::move(left), std::move(right));
    series result(size);
    for (int i = 0; i < size; ++i) result[i] = transformed[i] * factorial[i];
    return result;
}

}
