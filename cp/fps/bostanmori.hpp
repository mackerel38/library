#pragma once
#include <bits/stdc++.h>
#include "math/convolution.hpp"

namespace poe {

/// O(M(k) log n)。P(x)/Q(x)のx^nの係数をBostan-Mori法で返す。
/// Q[0]!=0かつP.size()<Q.size()。modはNTT可能な素数。
template <int mod>
staticmodint<mod> bostan_mori(
    std::vector<staticmodint<mod>> numerator,
    std::vector<staticmodint<mod>> denominator,
    long long index
) {
    assert(index >= 0 && !denominator.empty() && denominator[0] != 0);
    assert(numerator.size() < denominator.size());
    while (index > 0) {
        auto negative = denominator;
        for (std::size_t i = 1; i < negative.size(); i += 2) negative[i] = -negative[i];
        const auto product_p = convolution(numerator, negative);
        const auto product_q = convolution(denominator, negative);
        std::vector<staticmodint<mod>> next_p;
        std::vector<staticmodint<mod>> next_q;
        for (std::size_t i = static_cast<std::size_t>(index & 1);
             i < product_p.size(); i += 2) {
            next_p.push_back(product_p[i]);
        }
        for (std::size_t i = 0; i < product_q.size(); i += 2) {
            next_q.push_back(product_q[i]);
        }
        numerator = std::move(next_p);
        denominator = std::move(next_q);
        index >>= 1;
    }
    return numerator.empty() ? staticmodint<mod>{0} : numerator[0] / denominator[0];
}

/// O(M(k) log mask)。P/Qの係数a[i]についてiがmaskのsubmaskであるa[i]の総和を返す。
template<int mod>
staticmodint<mod> bostan_mori_submask_sum(
    std::vector<staticmodint<mod>> numerator,
    std::vector<staticmodint<mod>> denominator,
    unsigned long long mask
) {
    using mint = staticmodint<mod>;
    assert(!denominator.empty() && denominator[0] != 0);
    assert(numerator.size() < denominator.size());
    while (mask > 0) {
        auto negative = denominator;
        for (std::size_t index = 1; index < negative.size(); index += 2) {
            negative[index] = -negative[index];
        }
        const auto product_p = convolution(numerator, negative);
        const auto product_q = convolution(denominator, negative);
        std::vector<mint> next_p((product_p.size() + 1) / 2);
        std::vector<mint> next_q((product_q.size() + 1) / 2);
        for (std::size_t index = 0; 2 * index < product_p.size(); ++index) {
            next_p[index] = product_p[2 * index];
            if ((mask & 1) && 2 * index + 1 < product_p.size()) {
                next_p[index] += product_p[2 * index + 1];
            }
        }
        for (std::size_t index = 0; 2 * index < product_q.size(); ++index) {
            next_q[index] = product_q[2 * index];
        }
        numerator = std::move(next_p);
        denominator = std::move(next_q);
        mask >>= 1;
    }
    return numerator.empty() ? mint{} : numerator[0] / denominator[0];
}

/// O(M(k) log n)。固定係数k項線形漸化式のa[n]を畳み込みで求める。
/// a[i]=sum coefficient[j-1]*a[i-j]。modはNTT可能な素数。
template <int mod>
staticmodint<mod> fast_linear_recurrence(
    const std::vector<staticmodint<mod>>& initial,
    const std::vector<staticmodint<mod>>& coefficient,
    long long index
) {
    const int order = static_cast<int>(coefficient.size());
    assert(order > 0 && static_cast<int>(initial.size()) == order && index >= 0);
    if (index < order) return initial[index];
    std::vector<staticmodint<mod>> denominator(static_cast<std::size_t>(order + 1));
    denominator[0] = 1;
    for (int i = 0; i < order; ++i) denominator[i + 1] = -coefficient[i];
    auto numerator = convolution(initial, denominator);
    numerator.resize(static_cast<std::size_t>(order));
    return bostan_mori(std::move(numerator), std::move(denominator), index);
}

}
