#pragma once
#include <bits/stdc++.h>

namespace poe {

/// primetable(n): 0..nの最小素因数をO(n)で前計算し、素数判定・因数分解・約数列挙に使う。
struct primetable {
    std::vector<int> primes;

    /// O(n)。0<=value<=nの最小素因数を線形篩で構築する。
    explicit primetable(int limit) : least_(static_cast<std::size_t>(limit) + 1) {
        assert(limit >= 0);
        for (int value = 2; value <= limit; ++value) {
            if (least_[value] == 0) {
                least_[value] = value;
                primes.push_back(value);
            }
            for (int prime : primes) {
                if (prime > least_[value] || 1LL * value * prime > limit) break;
                least_[value * prime] = prime;
            }
        }
    }

    /// O(1)。table[value]: valueが素数か返す。
    bool operator[](int value) const {
        assert(0 <= value && value < static_cast<int>(least_.size()));
        return value >= 2 && least_[value] == value;
    }

    /// O(1)。table.least(value): valueの最小素因数を返す。value>=2。
    int least(int value) const {
        assert(2 <= value && value < static_cast<int>(least_.size()));
        return least_[value];
    }

    /// O(log value)。table.factorize(value): {{素因数,指数}}を昇順で返す。value>=1。
    std::vector<std::pair<int, int>> factorize(int value) const {
        assert(1 <= value && value < static_cast<int>(least_.size()));
        std::vector<std::pair<int, int>> result;
        while (value > 1) {
            const int prime = least_[value];
            int exponent = 0;
            while (value % prime == 0) value /= prime, ++exponent;
            result.emplace_back(prime, exponent);
        }
        return result;
    }

    /// O(tau(value) log tau(value))。table.divisors(value): 正の約数を昇順で返す。
    std::vector<int> divisors(int value) const {
        std::vector<int> result{1};
        for (const auto [prime, exponent] : factorize(value)) {
            const int previous = static_cast<int>(result.size());
            int power = 1;
            for (int count = 1; count <= exponent; ++count) {
                power *= prime;
                for (int i = 0; i < previous; ++i) result.push_back(result[i] * power);
            }
        }
        std::ranges::sort(result);
        return result;
    }

private:
    std::vector<int> least_;
};

namespace detail {

inline unsigned long long multiply_mod(
    unsigned long long left,
    unsigned long long right,
    unsigned long long mod
) {
    return static_cast<unsigned long long>(static_cast<__uint128_t>(left) * right % mod);
}

inline unsigned long long power_mod(
    unsigned long long base,
    unsigned long long exponent,
    unsigned long long mod
) {
    unsigned long long result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = multiply_mod(result, base, mod);
        base = multiply_mod(base, base, mod);
        exponent >>= 1;
    }
    return result;
}

inline bool miller_rabin(unsigned long long value) {
    if (value < 2) return false;
    for (unsigned long long prime : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (value % prime == 0) return value == prime;
    }
    unsigned long long odd = value - 1;
    const int shifts = std::countr_zero(odd);
    odd >>= shifts;
    for (unsigned long long base : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (base % value == 0) continue;
        unsigned long long current = power_mod(base % value, odd, value);
        if (current == 1 || current == value - 1) continue;
        bool composite = true;
        for (int count = 1; count < shifts; ++count) {
            current = multiply_mod(current, current, value);
            if (current == value - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

inline unsigned long long pollard_rho(unsigned long long value) {
    if (value % 2 == 0) return 2;
    if (value % 3 == 0) return 3;
    static std::mt19937_64 random(712367821);
    while (true) {
        const unsigned long long constant = random() % (value - 1) + 1;
        unsigned long long left = random() % value;
        unsigned long long right = left;
        unsigned long long divisor = 1;
        auto next = [&](unsigned long long current) {
            return static_cast<unsigned long long>(
                (static_cast<__uint128_t>(multiply_mod(current, current, value)) + constant) % value);
        };
        while (divisor == 1) {
            left = next(left);
            right = next(next(right));
            divisor = std::gcd(left > right ? left - right : right - left, value);
        }
        if (divisor != value) return divisor;
    }
}

inline void factorize_rec(unsigned long long value, std::vector<unsigned long long>& result) {
    if (value == 1) return;
    if (miller_rabin(value)) {
        result.push_back(value);
        return;
    }
    const unsigned long long divisor = pollard_rho(value);
    factorize_rec(divisor, result);
    factorize_rec(value / divisor, result);
}

}

/// O(log^3 n)程度。is_prime(n): 64bit符号なし整数nが素数か決定的Miller–Rabin法で判定する。
inline bool is_prime(unsigned long long value) {
    return detail::miller_rabin(value);
}

/// 期待O(n^(1/4))程度。factorize(n): 64bit整数nの{{素因数,指数}}を昇順で返す。n>=1。
inline std::vector<std::pair<unsigned long long, int>> factorize(unsigned long long value) {
    assert(value >= 1);
    std::vector<unsigned long long> factors;
    detail::factorize_rec(value, factors);
    std::ranges::sort(factors);
    std::vector<std::pair<unsigned long long, int>> result;
    for (const auto prime : factors) {
        if (result.empty() || result.back().first != prime) result.emplace_back(prime, 1);
        else ++result.back().second;
    }
    return result;
}

/// O(tau(n) log tau(n))。divisors(n): 64bit整数nの正の約数を昇順で返す。n>=1。
inline std::vector<unsigned long long> divisors(unsigned long long value) {
    std::vector<unsigned long long> result{1};
    for (const auto [prime, exponent] : factorize(value)) {
        const int previous = static_cast<int>(result.size());
        unsigned long long power = 1;
        for (int count = 1; count <= exponent; ++count) {
            power *= prime;
            for (int i = 0; i < previous; ++i) result.push_back(result[i] * power);
        }
    }
    std::ranges::sort(result);
    return result;
}

/// 期待O(|n|^(1/4)+tau(|n|) log tau(|n|))程度。signed_divisors(n): 非零整数nの正負の約数を昇順で返す。
inline std::vector<long long> signed_divisors(long long value) {
    assert(value != 0 && value != std::numeric_limits<long long>::min());
    const auto positive = divisors(static_cast<unsigned long long>(std::abs(value)));
    std::vector<long long> result;
    result.reserve(positive.size() * 2);
    for (auto iterator = positive.rbegin(); iterator != positive.rend(); ++iterator) {
        result.push_back(-static_cast<long long>(*iterator));
    }
    for (const auto divisor : positive) result.push_back(static_cast<long long>(divisor));
    return result;
}

}
