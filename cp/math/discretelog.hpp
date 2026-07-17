#pragma once
#include <bits/stdc++.h>
#include "math/math.hpp"

namespace poe {

namespace detail {

inline long long multiply_mod_ll(long long left, long long right, long long mod) {
    return static_cast<long long>(static_cast<__int128>(left) * right % mod);
}

inline long long power_mod_ll(long long base, long long exponent, long long mod) {
    long long result = 1 % mod;
    base = safe_mod(base, mod);
    while (exponent > 0) {
        if (exponent & 1) result = multiply_mod_ll(result, base, mod);
        base = multiply_mod_ll(base, base, mod);
        exponent >>= 1;
    }
    return result;
}

inline long long coprime_discrete_log(long long base, long long target, long long mod) {
    const long long block = static_cast<long long>(std::sqrt(static_cast<long double>(mod))) + 1;
    std::unordered_map<long long, long long> baby;
    baby.reserve(static_cast<std::size_t>(block * 2 + 1));
    long long current = 1;
    for (long long exponent = 0; exponent < block; ++exponent) {
        baby.try_emplace(current, exponent);
        current = multiply_mod_ll(current, base, mod);
    }
    const long long inverse_giant = inv_mod(power_mod_ll(base, block, mod), mod);
    current = target;
    for (long long giant = 0; giant <= block; ++giant) {
        if (const auto iterator = baby.find(current); iterator != baby.end()) {
            return giant * block + iterator->second;
        }
        current = multiply_mod_ll(current, inverse_giant, mod);
    }
    return -1;
}

}

/// O(sqrt(mod))時間・領域。discrete_log(a,b,mod): a^x=b(mod mod)となる最小x>=0。なければ-1。
inline long long discrete_log(long long base, long long target, long long mod) {
    assert(mod >= 1);
    base = detail::safe_mod(base, mod);
    target = detail::safe_mod(target, mod);
    if (mod == 1 || target == 1 % mod) return 0;
    long long multiplier = 1;
    long long removed = 0;
    while (true) {
        const long long divisor = std::gcd(base, mod);
        if (divisor == 1) break;
        if (target == multiplier) return removed;
        if (target % divisor != 0) return -1;
        target /= divisor;
        mod /= divisor;
        multiplier = detail::multiply_mod_ll(multiplier, base / divisor, mod);
        ++removed;
    }
    if (mod == 1) return removed;
    target = detail::multiply_mod_ll(target, inv_mod(multiplier, mod), mod);
    const long long remaining = detail::coprime_discrete_log(base, target, mod);
    return remaining == -1 ? -1 : removed + remaining;
}

}
