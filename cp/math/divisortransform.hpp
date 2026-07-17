#pragma once
#include <bits/stdc++.h>

namespace poe {

namespace detail {

inline std::vector<int> transform_primes(int n) {
    std::vector<bool> is_prime(static_cast<std::size_t>(n) + 1, true);
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;
    std::vector<int> primes;
    for (int value = 2; value <= n; ++value) {
        if (!is_prime[value]) continue;
        primes.push_back(value);
        if (1LL * value * value <= n) {
            for (int multiple = value * value; multiple <= n; multiple += value) is_prime[multiple] = false;
        }
    }
    return primes;
}

}

/// O(n log log n)。divisor_zeta(a): a[x]をsum_{d|x} original[d]へ変換する。a[0]は不使用。
template <class T>
void divisor_zeta(std::vector<T>& values) {
    const int n = static_cast<int>(values.size()) - 1;
    for (const int prime : detail::transform_primes(n)) {
        for (int value = 1; value * prime <= n; ++value) values[value * prime] += values[value];
    }
}

/// O(n log log n)。divisor_mobius(a): divisor_zetaの逆変換を行う。a[0]は不使用。
template <class T>
void divisor_mobius(std::vector<T>& values) {
    const int n = static_cast<int>(values.size()) - 1;
    for (const int prime : detail::transform_primes(n)) {
        for (int value = n / prime; value >= 1; --value) values[value * prime] -= values[value];
    }
}

/// O(n log log n)。multiple_zeta(a): a[x]をsum_{x|m} original[m]へ変換する。a[0]は不使用。
template <class T>
void multiple_zeta(std::vector<T>& values) {
    const int n = static_cast<int>(values.size()) - 1;
    for (const int prime : detail::transform_primes(n)) {
        for (int value = n / prime; value >= 1; --value) values[value] += values[value * prime];
    }
}

/// O(n log log n)。multiple_mobius(a): multiple_zetaの逆変換を行う。a[0]は不使用。
template <class T>
void multiple_mobius(std::vector<T>& values) {
    const int n = static_cast<int>(values.size()) - 1;
    for (const int prime : detail::transform_primes(n)) {
        for (int value = 1; value * prime <= n; ++value) values[value] -= values[value * prime];
    }
}

}
