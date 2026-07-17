#pragma once
#include <bits/stdc++.h>
#include "fps/fps.hpp"

namespace poe {

/// O((E(n)+M(n)) log n)。F(y)=0をNewton法で解き、y mod x^nを返す。
/// equation(y,n)はpair{F(y), dF/dy(y)}をそれぞれn項返す。constantはF(constant)=0 mod xを満たすこと。
template<int mod, class Equation>
fps<mod> newton_fps(int size, staticmodint<mod> constant, Equation equation) {
    using series = fps<mod>;
    assert(size >= 0);
    if (size == 0) return {};
    series result{constant};
    for (int length = 1; length < size; length <<= 1) {
        const int next = std::min(size, length << 1);
        result.resize(next);
        auto [value, derivative] = equation(result, next);
        value.resize(next);
        derivative.resize(next);
        assert(!derivative.empty() && derivative[0] != staticmodint<mod>{});
        result = (result - value.div(derivative, next)).prefix(next);
    }
    return result;
}

}
