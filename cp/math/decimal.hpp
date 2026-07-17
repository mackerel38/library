#pragma once
#include <bits/stdc++.h>
#include "math/math.hpp"

namespace poe {

/// 十進表記の同一数字run: decimalrun{digit,length}。
struct decimalrun {
    int digit;
    unsigned long long length;
};

namespace detail {

inline unsigned long long decimal_mul_mod(unsigned long long first,
                                          unsigned long long second,
                                          unsigned long long modulus) {
    return static_cast<unsigned long long>(
        static_cast<unsigned __int128>(first) * second % modulus
    );
}

}

/// O(K log L)。RLE十進整数をmodulusで割った余りを返す。
inline unsigned long long decimal_runs_mod(
    const std::vector<decimalrun>& runs,
    unsigned long long modulus
) {
    assert(modulus >= 1);
    if (modulus == 1) return 0;
    unsigned long long value = 0;
    for (const auto [digit, length] : runs) {
        assert(0 <= digit && digit <= 9 && length >= 1);
        unsigned long long result_multiply = 1;
        unsigned long long result_add = 0;
        unsigned long long power_multiply = 10 % modulus;
        unsigned long long power_add = digit % modulus;
        for (unsigned long long exponent = length; exponent > 0; exponent >>= 1) {
            if (exponent & 1) {
                result_add = (detail::decimal_mul_mod(power_multiply, result_add, modulus)
                              + power_add) % modulus;
                result_multiply = detail::decimal_mul_mod(
                    power_multiply, result_multiply, modulus
                );
            }
            power_add = (detail::decimal_mul_mod(power_multiply, power_add, modulus)
                         + power_add) % modulus;
            power_multiply = detail::decimal_mul_mod(
                power_multiply, power_multiply, modulus
            );
        }
        value = (detail::decimal_mul_mod(result_multiply, value, modulus)
                 + result_add) % modulus;
    }
    return value;
}

/// O(K log L)。RLE十進整数Nについてfloor(N/divisor) mod modulusを返す。
inline unsigned long long decimal_runs_div_mod(
    const std::vector<decimalrun>& runs,
    unsigned long long divisor,
    unsigned long long modulus
) {
    assert(divisor >= 1 && modulus >= 1);
    if (modulus == 1) return 0;
    assert(divisor <= std::numeric_limits<unsigned long long>::max() / modulus);
    return decimal_runs_mod(runs, divisor * modulus) / divisor;
}

/// O(n d log n)。base進表記を連結した値がmodulusの倍数となる順序付きpair数を返す。
inline long long count_divisible_concatenations(
    const std::vector<unsigned long long>& values,
    unsigned long long modulus,
    unsigned long long base = 10
) {
    assert(modulus >= 1 && base >= 2);
    int max_digits = 1;
    std::vector<int> digits(values.size());
    for (int i = 0; i < static_cast<int>(values.size()); ++i) {
        assert(values[i] >= 1);
        unsigned long long current = values[i];
        while (current >= base) {
            current /= base;
            ++digits[i];
        }
        ++digits[i];
        max_digits = std::max(max_digits, digits[i]);
    }
    std::vector<std::vector<unsigned long long>> remainder(max_digits + 1);
    for (int i = 0; i < static_cast<int>(values.size()); ++i) {
        remainder[digits[i]].push_back(values[i] % modulus);
    }
    for (auto& group : remainder) std::ranges::sort(group);

    long long result = 0;
    for (unsigned long long value : values) {
        unsigned long long power = 1 % modulus;
        for (int length = 1; length <= max_digits; ++length) {
            power = detail::decimal_mul_mod(power, base % modulus, modulus);
            const unsigned long long shifted = detail::decimal_mul_mod(
                value % modulus, power, modulus);
            const unsigned long long target = (modulus - shifted) % modulus;
            const auto [first, last] = std::ranges::equal_range(remainder[length], target);
            result += last - first;
        }
    }
    return result;
}

/// O(|pattern|d+d 10^(d/2)log modulus)。patternを含む最小の正の十進modulus倍数を返す。modulus<=1e9。
inline std::string smallest_multiple_containing(int modulus, const std::string& pattern) {
    assert(1 <= modulus && modulus <= 1'000'000'000);
    assert(!pattern.empty() && pattern[0] != '0');
    long long pattern_remainder = 0;
    for (const char digit : pattern) {
        assert('0' <= digit && digit <= '9');
        pattern_remainder = (pattern_remainder * 10 + digit - '0') % modulus;
    }

    int extra_digits = 0;
    long long power = 1;
    while (power < modulus) {
        power *= 10;
        ++extra_digits;
    }
    std::vector<long long> power10(extra_digits + 1, 1);
    for (int i = 1; i <= extra_digits; ++i) power10[i] = power10[i - 1] * 10;

    auto padded = [](long long value, int length) {
        std::string result(length, '0');
        for (int index = length - 1; index >= 0; --index) {
            result[index] = static_cast<char>('0' + value % 10);
            value /= 10;
        }
        return result;
    };
    auto better = [](const std::string& first, const std::string& second) {
        return second.empty() || std::pair{first.size(), first} < std::pair{second.size(), second};
    };

    std::string answer;
    for (int suffix_length = 0; suffix_length <= extra_digits; ++suffix_length) {
        const int prefix_width = extra_digits - suffix_length;
        const long long suffix_power_mod = pow_mod(10, suffix_length, modulus);
        const long long shifted_pattern = static_cast<long long>(
            static_cast<__int128>(pattern_remainder) * suffix_power_mod % modulus
        );
        const long long prefix_power = pow_mod(
            10, static_cast<long long>(pattern.size()) + suffix_length, modulus
        );
        long long best_prefix = -1;
        long long best_suffix = -1;

        if (suffix_length <= prefix_width) {
            const long long gcd = std::gcd(prefix_power, static_cast<long long>(modulus));
            const long long reduced_modulus = modulus / gcd;
            const long long inverse = inv_mod(prefix_power / gcd, reduced_modulus);
            for (long long suffix = 0; suffix < power10[suffix_length]; ++suffix) {
                const long long constant = (shifted_pattern + suffix) % modulus;
                const long long target = (modulus - constant) % modulus;
                if (target % gcd != 0) continue;
                const long long prefix = static_cast<long long>(
                    static_cast<__int128>(target / gcd) * inverse % reduced_modulus
                );
                if (best_prefix == -1
                    || std::pair{prefix, suffix} < std::pair{best_prefix, best_suffix}) {
                    best_prefix = prefix;
                    best_suffix = suffix;
                }
            }
        } else {
            for (long long prefix = 0; prefix < power10[prefix_width]; ++prefix) {
                const long long prefix_part = static_cast<long long>(
                    static_cast<__int128>(prefix) * prefix_power % modulus
                );
                const long long suffix = (modulus - (prefix_part + shifted_pattern) % modulus) % modulus;
                if (suffix >= power10[suffix_length]) continue;
                best_prefix = prefix;
                best_suffix = suffix;
                break;
            }
        }
        if (best_prefix == -1) continue;
        const std::string candidate = (best_prefix == 0 ? std::string{} : std::to_string(best_prefix))
            + pattern + padded(best_suffix, suffix_length);
        if (better(candidate, answer)) answer = candidate;
    }
    assert(!answer.empty());
    return answer;
}

}
