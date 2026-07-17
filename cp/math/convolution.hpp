#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"

namespace poe {

namespace detail {

/// O(sqrt(mod))。素数modの原始根を返す。
inline int primitive_root(int mod) {
    if (mod == 2) {
        return 1;
    }
    if (mod == 754974721) {
        return 11;
    }
    if (mod == 167772161 || mod == 469762049 || mod == 998244353) {
        return 3;
    }
    std::vector<int> factors;
    int value = mod - 1;
    for (int prime = 2; 1LL * prime * prime <= value; ++prime) {
        if (value % prime == 0) {
            factors.push_back(prime);
            while (value % prime == 0) {
                value /= prime;
            }
        }
    }
    if (value > 1) {
        factors.push_back(value);
    }
    for (int root = 2;; ++root) {
        bool ok = true;
        for (int factor : factors) {
            if (pow_mod(root, (mod - 1) / factor, mod) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) {
            return root;
        }
    }
}

/// O(n log n)。aをNTTまたは逆NTTする。
template <int mod>
void ntt(std::vector<staticmodint<mod>>& a, bool inverse) {
    const int n = static_cast<int>(a.size());
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) {
            std::swap(a[i], a[j]);
        }
    }
    const staticmodint<mod> primitive = primitive_root(mod);
    for (int length = 2; length <= n; length <<= 1) {
        staticmodint<mod> root = primitive.pow((mod - 1) / length);
        if (inverse) {
            root = root.inv();
        }
        for (int start = 0; start < n; start += length) {
            staticmodint<mod> power = 1;
            for (int offset = 0; offset < length / 2; ++offset) {
                const auto left = a[start + offset];
                const auto right = a[start + offset + length / 2] * power;
                a[start + offset] = left + right;
                a[start + offset + length / 2] = left - right;
                power *= root;
            }
        }
    }
    if (inverse) {
        const staticmodint<mod> inverse_n = staticmodint<mod>(n).inv();
        for (auto& value : a) {
            value *= inverse_n;
        }
    }
}

}

/// O(n log n)。modint列a,bの畳み込みを返す。modはNTT可能な素数。
template <int mod>
std::vector<staticmodint<mod>> convolution(std::vector<staticmodint<mod>> a,
                                           std::vector<staticmodint<mod>> b) {
    if (a.empty() || b.empty()) {
        return {};
    }
    if (std::min(a.size(), b.size()) <= 60) {
        std::vector<staticmodint<mod>> result(a.size() + b.size() - 1);
        for (std::size_t i = 0; i < a.size(); ++i) {
            for (std::size_t j = 0; j < b.size(); ++j) {
                result[i + j] += a[i] * b[j];
            }
        }
        return result;
    }
    int size = 1;
    while (size < static_cast<int>(a.size() + b.size() - 1)) {
        size <<= 1;
    }
    if ((mod - 1) % size != 0) {
        constexpr long long mod1 = 754974721;
        constexpr long long mod2 = 167772161;
        constexpr long long mod3 = 469762049;
        const __int128 all_moduli = static_cast<__int128>(mod1) * mod2 * mod3;
        const __int128 coefficient_bound = static_cast<__int128>(std::min(a.size(), b.size()))
            * (mod - 1LL) * (mod - 1LL);
        assert(coefficient_bound < all_moduli);
        std::vector<staticmodint<mod1>> a1(a.size()), b1(b.size());
        std::vector<staticmodint<mod2>> a2(a.size()), b2(b.size());
        std::vector<staticmodint<mod3>> a3(a.size()), b3(b.size());
        for (std::size_t i = 0; i < a.size(); ++i) {
            a1[i] = a[i].val();
            a2[i] = a[i].val();
            a3[i] = a[i].val();
        }
        for (std::size_t i = 0; i < b.size(); ++i) {
            b1[i] = b[i].val();
            b2[i] = b[i].val();
            b3[i] = b[i].val();
        }
        const auto c1 = convolution(std::move(a1), std::move(b1));
        const auto c2 = convolution(std::move(a2), std::move(b2));
        const auto c3 = convolution(std::move(a3), std::move(b3));
        const long long inv12 = inv_mod(mod1, mod2);
        const long long inv123 = inv_mod(
            static_cast<long long>(static_cast<__int128>(mod1) * mod2 % mod3), mod3);
        const __int128 product12 = static_cast<__int128>(mod1) * mod2;
        std::vector<staticmodint<mod>> result(c1.size());
        for (std::size_t i = 0; i < result.size(); ++i) {
            __int128 value = c1[i].val();
            long long factor = detail::safe_mod(c2[i].val() - static_cast<long long>(value % mod2), mod2);
            factor = static_cast<long long>(static_cast<__int128>(factor) * inv12 % mod2);
            value += static_cast<__int128>(mod1) * factor;
            long long factor3 = detail::safe_mod(c3[i].val() - static_cast<long long>(value % mod3), mod3);
            factor3 = static_cast<long long>(static_cast<__int128>(factor3) * inv123 % mod3);
            value += product12 * factor3;
            result[i] = static_cast<long long>(value % mod);
        }
        return result;
    }
    const std::size_t result_size = a.size() + b.size() - 1;
    a.resize(size);
    b.resize(size);
    detail::ntt(a, false);
    detail::ntt(b, false);
    for (int i = 0; i < size; ++i) {
        a[i] *= b[i];
    }
    detail::ntt(a, true);
    a.resize(result_size);
    return a;
}

/// O(n log n)。整数列a,bをmodで畳み込み、[0,mod)の整数列で返す。modはNTT可能な素数。
template <int mod = 998244353, class T,
          std::enable_if_t<std::is_integral_v<T>, int> = 0>
std::vector<T> convolution(const std::vector<T>& a, const std::vector<T>& b) {
    std::vector<staticmodint<mod>> left(a.begin(), a.end());
    std::vector<staticmodint<mod>> right(b.begin(), b.end());
    const auto product = convolution(std::move(left), std::move(right));
    std::vector<T> result(product.size());
    for (std::size_t i = 0; i < product.size(); ++i) {
        result[i] = static_cast<T>(product[i].val());
    }
    return result;
}

/// O(n log n)。long long列を整数として畳み込む。全係数がlong long範囲内であること。
inline std::vector<long long> convolution_ll(const std::vector<long long>& a,
                                             const std::vector<long long>& b) {
    if (a.empty() || b.empty()) {
        return {};
    }
    constexpr long long mod1 = 754974721;
    constexpr long long mod2 = 167772161;
    constexpr long long mod3 = 469762049;
    const auto c1 = convolution<mod1>(a, b);
    const auto c2 = convolution<mod2>(a, b);
    const auto c3 = convolution<mod3>(a, b);
    const long long inv12 = inv_mod(mod1, mod2);
    const long long inv123 = inv_mod(static_cast<long long>(
        static_cast<__int128>(mod1) * mod2 % mod3), mod3);
    const __int128 product12 = static_cast<__int128>(mod1) * mod2;
    const __int128 all_moduli = product12 * mod3;
    std::vector<long long> result(c1.size());
    for (std::size_t i = 0; i < result.size(); ++i) {
        __int128 value = c1[i];
        long long factor = detail::safe_mod(c2[i] - static_cast<long long>(value % mod2), mod2);
        factor = static_cast<long long>(static_cast<__int128>(factor) * inv12 % mod2);
        value += static_cast<__int128>(mod1) * factor;
        long long factor3 =
            detail::safe_mod(c3[i] - static_cast<long long>(value % mod3), mod3);
        factor3 = static_cast<long long>(static_cast<__int128>(factor3) * inv123 % mod3);
        value += product12 * factor3;
        if (value > all_moduli / 2) {
            value -= all_moduli;
        }
        assert(std::numeric_limits<long long>::min() <= value &&
               value <= std::numeric_limits<long long>::max());
        result[i] = static_cast<long long>(value);
    }
    return result;
}

}
