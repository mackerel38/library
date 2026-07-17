#pragma once
#include <bits/stdc++.h>
#include "fps/fps.hpp"

namespace poe {

/// O(n * nonzero(f))。sparse_power(f,k,n): 非零項が少ないfのf^k mod x^nを返す。
/// k>=0、先頭非零係数を除いた後の定数項が非0、n<mod。
template<int mod>
fps<mod> sparse_power(const fps<mod>& polynomial, long long exponent, int size) {
    using mint = staticmodint<mod>;
    using series = fps<mod>;
    assert(exponent >= 0 && 0 <= size && size < mod);
    series answer(static_cast<std::size_t>(size));
    if (size == 0) return answer;
    if (exponent == 0) {
        answer[0] = 1;
        return answer;
    }
    int order = 0;
    while (order < polynomial.size_int() && polynomial[order] == mint{}) ++order;
    if (order == polynomial.size_int() || order > (size - 1) / exponent) return answer;
    const int shift = static_cast<int>(order * exponent);
    std::vector<std::pair<int, mint>> terms;
    for (int i = order; i < polynomial.size_int() && i - order < size - shift; ++i) {
        if (polynomial[i] != mint{}) terms.push_back({i - order, polynomial[i]});
    }
    const mint constant = terms[0].second;
    series normalized(static_cast<std::size_t>(size - shift));
    normalized[0] = constant.pow(exponent);
    for (int degree = 1; degree < size - shift; ++degree) {
        mint value{};
        for (const auto [index, coefficient] : terms) {
            if (index == 0 || index > degree) continue;
            value += mint{(exponent + 1) * index - degree}
                * coefficient * normalized[degree - index];
        }
        normalized[degree] = value / (mint{degree} * constant);
    }
    for (int i = 0; i < normalized.size_int(); ++i) answer[shift + i] = normalized[i];
    return answer;
}

/// O(n)。multiply_binomial(f,d,c,n): f(x)(1+c*x^d) mod x^nを返す。
template<int mod>
fps<mod> multiply_binomial(fps<mod> series, int degree, staticmodint<mod> coefficient, int size) {
    assert(degree > 0 && size >= 0);
    series.resize(size);
    for (int i = size - 1; i >= degree; --i) series[i] += coefficient * series[i - degree];
    return series;
}

/// O(n)。divide_binomial(f,d,c,n): f(x)/(1+c*x^d) mod x^nを返す。
template<int mod>
fps<mod> divide_binomial(fps<mod> series, int degree, staticmodint<mod> coefficient, int size) {
    assert(degree > 0 && size >= 0);
    series.resize(size);
    for (int i = degree; i < size; ++i) series[i] -= coefficient * series[i - degree];
    return series;
}

}
