#pragma once
#include <bits/stdc++.h>
#include "fps/fps.hpp"

namespace poe {

/// O(M(n))。binomial_convolution(a,b)[n]=sum C(n,k)a[k]b[n-k]を返す。
template<int mod>
fps<mod> binomial_convolution(const fps<mod>& left, const fps<mod>& right) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    if (left.empty() || right.empty()) return {};
    const int size = left.size_int() + right.size_int() - 1;
    assert(size < mod);
    std::vector<mint> factorial(size, 1), inverse_factorial(size, 1);
    for (int i = 1; i < size; ++i) factorial[i] = factorial[i - 1] * i;
    inverse_factorial[size - 1] = factorial[size - 1].inv();
    for (int i = size - 1; i > 0; --i) inverse_factorial[i - 1] = inverse_factorial[i] * i;
    series normalized_left = left;
    series normalized_right = right;
    for (int i = 0; i < left.size_int(); ++i) normalized_left[i] *= inverse_factorial[i];
    for (int i = 0; i < right.size_int(); ++i) normalized_right[i] *= inverse_factorial[i];
    series result = normalized_left * normalized_right;
    for (int i = 0; i < size; ++i) result[i] *= factorial[i];
    return result;
}

/// O(M(n))。binomial_transform(a,c)[n]=sum C(n,k)a[k]c^(n-k)を返す。
template<int mod>
fps<mod> binomial_transform(const fps<mod>& values, staticmodint<mod> coefficient) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    const int size = values.size_int();
    assert(size < mod);
    std::vector<mint> factorial(size, 1), inverse_factorial(size, 1);
    for (int i = 1; i < size; ++i) factorial[i] = factorial[i - 1] * i;
    if (size > 0) inverse_factorial[size - 1] = factorial[size - 1].inv();
    for (int i = size - 1; i > 0; --i) inverse_factorial[i - 1] = inverse_factorial[i] * i;
    series left(size), right(size);
    mint power = 1;
    for (int i = 0; i < size; ++i) {
        left[i] = values[i] * inverse_factorial[i];
        right[i] = power * inverse_factorial[i];
        power *= coefficient;
    }
    series product = (left * right).prefix(size);
    for (int i = 0; i < size; ++i) product[i] *= factorial[i];
    return product;
}

/// O(M(n))。upper_binomial_transform(a,c)[k]=sum_{n>=k} C(n,k)a[n]c^(n-k)を返す。
template<int mod>
fps<mod> upper_binomial_transform(const fps<mod>& values, staticmodint<mod> coefficient) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    const int size = values.size_int();
    assert(size < mod);
    if (size == 0) return {};
    std::vector<mint> factorial(size, 1), inverse_factorial(size, 1);
    for (int i = 1; i < size; ++i) factorial[i] = factorial[i - 1] * i;
    inverse_factorial[size - 1] = factorial[size - 1].inv();
    for (int i = size - 1; i > 0; --i) inverse_factorial[i - 1] = inverse_factorial[i] * i;
    series left(size), right(size);
    mint power = 1;
    for (int i = 0; i < size; ++i) {
        left[size - 1 - i] = values[i] * factorial[i];
        right[i] = power * inverse_factorial[i];
        power *= coefficient;
    }
    const series product = left * right;
    series result(size);
    for (int k = 0; k < size; ++k) {
        result[k] = product[size - 1 - k] * inverse_factorial[k];
    }
    return result;
}

}
