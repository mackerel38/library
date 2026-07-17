#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(log exponent)。Monoid::identity()を単位元としてvalue^exponentを返す。
template<class Monoid, std::unsigned_integral Integer>
Monoid monoid_power(Monoid value, Integer exponent) {
    Monoid result = Monoid::identity();
    while (exponent > 0) {
        if (exponent & 1) result *= value;
        value *= value;
        exponent >>= 1;
    }
    return result;
}

/// powersummonoid<T>::x(a), y(b)をfloor_productへ渡すとsum a^i*b^floorを得る。
template<class T>
struct powersummonoid {
    T product{1};
    T sum{};

    /// O(1)。積の単位元を返す。
    static powersummonoid identity() { return {}; }

    /// O(1)。添字を1進め、現在項を和へ加える元を返す。
    static powersummonoid x(const T& ratio) { return {ratio, T{1}}; }

    /// O(1)。floor値を1進める元を返す。
    static powersummonoid y(const T& ratio) { return {ratio, T{}}; }

    /// O(1)。処理列を連結する。
    powersummonoid& operator*=(const powersummonoid& right) {
        sum += product * right.sum;
        product *= right.product;
        return *this;
    }

    friend powersummonoid operator*(powersummonoid left, const powersummonoid& right) {
        return left *= right;
    }

    /// O(1)。累積和を返す。
    const T& answer() const noexcept { return sum; }
};

/// O(log(n+m+a+b))。格子境界floor((a*i+b)/m)に沿うx,yのモノイド積を返す。
template<class Monoid, std::unsigned_integral Integer>
Monoid floor_product(Integer n, Integer m, Integer a, Integer b, Monoid x, Monoid y) {
    assert(m > 0);
    x *= monoid_power(y, a / m);
    a %= m;
    const Monoid prefix = monoid_power(y, b / m);
    b %= m;
    const __uint128_t wide_top = static_cast<__uint128_t>(a) * n + b;
    assert(wide_top <= std::numeric_limits<Integer>::max());
    const Integer top = static_cast<Integer>(wide_top);
    if (top < m) return prefix * monoid_power(x, n);
    return prefix
         * floor_product(top / m - 1, a, m, m + a - b - 1, y, x)
         * y
         * monoid_power(x, top % m / a);
}

}
