#pragma once
#include <bits/stdc++.h>
#include "fps/polynomial.hpp"

namespace poe {

/// O(M(n))。falling_factorial_polynomial(n)=prod_{i=0}^{n-1}(x-i)を返す。
template <int mod>
fps<mod> falling_factorial_polynomial(int size) {
    assert(0 <= size && size < mod);
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    if (size == 0) return series{1};
    const int half = size / 2;
    series left = falling_factorial_polynomial<mod>(half);
    series result = left * taylor_shift(left, mint{-half});
    if (size & 1) {
        series extended(static_cast<std::size_t>(size) + 1);
        for (int degree = 0; degree < size; ++degree) {
            extended[degree] -= result[degree] * mint{size - 1};
            extended[degree + 1] += result[degree];
        }
        result = std::move(extended);
    }
    return result;
}

/// O(M(n))。符号付き第一種Stirling数s(n,k)をk=0..nの順に返す。
template <int mod>
fps<mod> signed_stirling_first_row(int n) {
    return falling_factorial_polynomial<mod>(n);
}

/// O(M(n)+n log n)。第二種Stirling数S(n,k)をk=0..nの順に返す。
template <int mod>
fps<mod> stirling_second_row(int n) {
    assert(0 <= n && n < mod);
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    std::vector<mint> factorial(static_cast<std::size_t>(n) + 1, 1);
    std::vector<mint> inverse_factorial(static_cast<std::size_t>(n) + 1, 1);
    for (int value = 1; value <= n; ++value) {
        factorial[value] = factorial[value - 1] * value;
    }
    inverse_factorial[n] = factorial[n].inv();
    for (int value = n; value >= 1; --value) {
        inverse_factorial[value - 1] = inverse_factorial[value] * value;
    }
    series powers(static_cast<std::size_t>(n) + 1);
    series signs(static_cast<std::size_t>(n) + 1);
    for (int value = 0; value <= n; ++value) {
        powers[value] = mint{value}.pow(n) * inverse_factorial[value];
        signs[value] = (value & 1 ? mint{-1} : mint{1}) *
                       inverse_factorial[value];
    }
    return (powers * signs).prefix(n + 1);
}

}
