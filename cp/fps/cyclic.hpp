#pragma once
#include <bits/stdc++.h>
#include "math/convolution.hpp"

namespace poe {

/// O(n log n)。cyclic_convolution(a,b): 同じ長さnの列を巡回畳み込みする。
template<int mod>
std::vector<staticmodint<mod>> cyclic_convolution(
    const std::vector<staticmodint<mod>>& left,
    const std::vector<staticmodint<mod>>& right
) {
    assert(!left.empty() && left.size() == right.size());
    const int size = static_cast<int>(left.size());
    auto product = convolution(left, right);
    product.resize(static_cast<std::size_t>(2 * size - 1));
    for (int index = size; index < 2 * size - 1; ++index) {
        product[index - size] += product[index];
    }
    product.resize(size);
    return product;
}

/// 2冪長はO(n log n)、一般長はO(M(n) log k)。cyclic_power(a,k)で巡回畳み込みのa^kを返す。
/// k>=0。2冪長かつmod-1の約数ならNTT領域で直接累乗する。
template<int mod>
std::vector<staticmodint<mod>> cyclic_power(
    std::vector<staticmodint<mod>> values,
    long long exponent
) {
    using mint = staticmodint<mod>;
    const int size = static_cast<int>(values.size());
    assert(size > 0 && exponent >= 0);
    if (
        std::has_single_bit(static_cast<unsigned int>(size))
        && (mod - 1) % size == 0
    ) {
        detail::ntt(values, false);
        for (mint& value : values) value = value.pow(exponent);
        detail::ntt(values, true);
        return values;
    }
    std::vector<mint> result(size);
    result[0] = 1;
    while (exponent > 0) {
        if (exponent & 1) result = cyclic_convolution<mod>(result, values);
        exponent >>= 1;
        if (exponent > 0) values = cyclic_convolution<mod>(values, values);
    }
    return result;
}

}
