#pragma once
#include <bits/stdc++.h>

namespace poe {

namespace detail {

/// O(1)。xを[0, mod)へ正規化する。
inline long long safe_mod(long long x, long long mod) {
    x %= mod;
    if (x < 0) {
        x += mod;
    }
    return x;
}

/// O(log mod)。g=gcd(a, mod)とa*x≡g(mod)を満たすxを返す。
inline std::pair<long long, long long> inv_gcd(long long a, long long mod) {
    a = safe_mod(a, mod);
    if (a == 0) {
        return {mod, 0};
    }
    long long s = mod;
    long long t = a;
    long long m0 = 0;
    long long m1 = 1;
    while (t) {
        const long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;
        std::swap(s, t);
        std::swap(m0, m1);
    }
    if (m0 < 0) {
        m0 += mod / s;
    }
    return {s, m0};
}

/// O(log mod)。非負引数のfloor sumを符号なし64bitで返す。
inline unsigned long long floor_sum_unsigned(unsigned long long n,
                                              unsigned long long mod,
                                              unsigned long long a,
                                              unsigned long long b) {
    unsigned long long answer = 0;
    while (true) {
        if (a >= mod) {
            answer += n * (n - 1) / 2 * (a / mod);
            a %= mod;
        }
        if (b >= mod) {
            answer += n * (b / mod);
            b %= mod;
        }
        const unsigned long long top = a * n + b;
        if (top < mod) {
            break;
        }
        n = top / mod;
        b = top % mod;
        std::swap(mod, a);
    }
    return answer;
}

}

/// O(log exponent)。base^exponent mod modを返す。exponent>=0、mod>=1。
inline long long pow_mod(long long base, long long exponent, int mod) {
    assert(exponent >= 0);
    assert(mod >= 1);
    if (mod == 1) {
        return 0;
    }
    unsigned long long value = static_cast<unsigned long long>(detail::safe_mod(base, mod));
    unsigned long long result = 1;
    const unsigned long long modulus = static_cast<unsigned long long>(mod);
    while (exponent > 0) {
        if (exponent & 1) {
            result = result * value % modulus;
        }
        value = value * value % modulus;
        exponent >>= 1;
    }
    return static_cast<long long>(result);
}

/// O(log mod)。xのmod逆元を[0, mod)で返す。gcd(x, mod)=1、mod>=1。
inline long long inv_mod(long long x, long long mod) {
    assert(mod >= 1);
    const auto [gcd, inverse] = detail::inv_gcd(x, mod);
    assert(gcd == 1);
    return inverse;
}

/// O(n log lcm)。x≡remainders[i](mod moduli[i])を解き、{x,lcm}を返す。解なしは{0,0}。
inline std::pair<long long, long long> crt(const std::vector<long long>& remainders,
                                          const std::vector<long long>& moduli) {
    assert(remainders.size() == moduli.size());
    long long result = 0;
    long long lcm = 1;
    for (std::size_t i = 0; i < remainders.size(); ++i) {
        assert(moduli[i] >= 1);
        long long remainder = detail::safe_mod(remainders[i], moduli[i]);
        long long modulus = moduli[i];
        if (lcm < modulus) {
            std::swap(lcm, modulus);
            std::swap(result, remainder);
        }
        if (lcm % modulus == 0) {
            if (result % modulus != remainder) {
                return {0, 0};
            }
            continue;
        }
        const auto [gcd, inverse] = detail::inv_gcd(lcm, modulus);
        const long long difference = remainder - result;
        if (difference % gcd != 0) {
            return {0, 0};
        }
        const long long quotient_modulus = modulus / gcd;
        const long long factor = static_cast<long long>(
            static_cast<__int128>(difference / gcd) * inverse % quotient_modulus);
        const long long normalized_factor = detail::safe_mod(factor, quotient_modulus);
        result += lcm * normalized_factor;
        lcm *= quotient_modulus;
        if (result < 0) {
            result += lcm;
        }
    }
    return {result, lcm};
}

/// O(log mod)。sum floor((a*i+b)/mod)をmod 2^64で返す。0<=n<2^32、1<=mod<2^32。
inline long long floor_sum(long long n, long long mod, long long a, long long b) {
    assert(n >= 0);
    assert(n < (1LL << 32));
    assert(1 <= mod && mod < (1LL << 32));
    unsigned long long answer = 0;
    if (a < 0) {
        const unsigned long long normalized =
            static_cast<unsigned long long>(detail::safe_mod(a, mod));
        answer -= static_cast<unsigned long long>(n) * (n - 1) / 2 *
                  ((normalized - static_cast<unsigned long long>(a)) /
                   static_cast<unsigned long long>(mod));
        a = static_cast<long long>(normalized);
    }
    if (b < 0) {
        const unsigned long long normalized =
            static_cast<unsigned long long>(detail::safe_mod(b, mod));
        answer -= static_cast<unsigned long long>(n) *
                  ((normalized - static_cast<unsigned long long>(b)) /
                   static_cast<unsigned long long>(mod));
        b = static_cast<long long>(normalized);
    }
    answer += detail::floor_sum_unsigned(
        static_cast<unsigned long long>(n), static_cast<unsigned long long>(mod),
        static_cast<unsigned long long>(a), static_cast<unsigned long long>(b));
    return static_cast<long long>(answer);
}

/// O(1)。非負64bit整数xに対してfloor(sqrt(x))を誤差なく返す。
inline unsigned long long floor_sqrt(unsigned long long x) {
    unsigned long long result = static_cast<unsigned long long>(
        std::sqrt(static_cast<long double>(x))
    );
    while (static_cast<unsigned __int128>(result) * result > x) --result;
    while (static_cast<unsigned __int128>(result + 1) * (result + 1) <= x) ++result;
    return result;
}

/// O(1)。xが平方数ならtrueを返す。
inline bool is_square(unsigned long long x) {
    const auto root = floor_sqrt(x);
    return root * root == x;
}

}
