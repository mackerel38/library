#pragma once
#include <bits/stdc++.h>
#include "math/floorproduct.hpp"
#include "math/math.hpp"
#include "math/modint.hpp"

namespace poe {

/// O(log(n+m+a+b+mod))。sum base^((a*i+b) mod m), 0<=i<nをmodで返す。
inline long long sum_power_mod_linear(
    unsigned long long n,
    unsigned long long m,
    unsigned long long a,
    unsigned long long b,
    unsigned long long base,
    int modulus
) {
    assert(m >= 1 && modulus >= 1 && a < m && b < m && base < static_cast<unsigned>(modulus));
    int coprime_modulus = modulus;
    while (true) {
        const int common = std::gcd(coprime_modulus, static_cast<int>(base));
        if (common == 1) break;
        coprime_modulus /= common;
    }

    using mint = dynamicmodint<-429>;
    mint::set_mod(coprime_modulus);
    using monoid = powersummonoid<mint>;
    const mint forward = mint{base}.pow(a);
    const mint wrap = mint{base}.pow(m).inv();
    const long long coprime_answer = (
        floor_product(n, m, a, b, monoid::x(forward), monoid::y(wrap)).answer()
        * mint{base}.pow(b)
    ).val();

    const int nilpotent_modulus = modulus / coprime_modulus;
    unsigned long long nilpotent_answer = 0;
    if (nilpotent_modulus > 1) {
        const unsigned long long divisor = std::gcd(a, m);
        const unsigned long long reduced_a = a / divisor;
        const unsigned long long reduced_m = m / divisor;
        const unsigned long long inverse = inv_mod(reduced_a, reduced_m);
        unsigned long long power = 1 % nilpotent_modulus;
        for (unsigned long long value = 0; value < m && power != 0; ++value) {
            if (value % divisor == b % divisor) {
                const unsigned long long first = ((value + m - b) / divisor * inverse) % reduced_m;
                if (first < n) {
                    const unsigned long long count = (n + reduced_m - first - 1) / reduced_m;
                    nilpotent_answer = (nilpotent_answer + count * power) % nilpotent_modulus;
                }
            }
            power = power * base % nilpotent_modulus;
        }
    }

    return crt(
        {coprime_answer, static_cast<long long>(nilpotent_answer)},
        {coprime_modulus, nilpotent_modulus}
    ).first;
}

}
