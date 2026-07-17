#pragma once
#include <bits/stdc++.h>

namespace poe {

/// fraction{numerator, denominator}: 正の分母を持つ64 bit有理数を表す。
struct fraction {
    long long numerator;
    long long denominator;
};

/// O(log max(a,b,c,d))。simplest_fraction_between(a,b,c,d): a/b<p/q<c/dで分母最小のp/qを返す。
inline fraction simplest_fraction_between(
    long long a,
    long long b,
    long long c,
    long long d
) {
    assert(a >= 0 && b > 0 && c > 0 && d > 0);
    assert(static_cast<__int128>(a) * d < static_cast<__int128>(c) * b);
    auto solve = [&](auto&& self, long long left_numerator, long long left_denominator,
                     long long right_numerator, long long right_denominator) -> fraction {
        const long long integer_part = left_numerator / left_denominator;
        left_numerator -= integer_part * left_denominator;
        right_numerator -= integer_part * right_denominator;
        if (right_numerator > right_denominator) return {integer_part + 1, 1};
        const auto inverted = self(
            self,
            right_denominator,
            right_numerator,
            left_denominator,
            left_numerator
        );
        const __int128 numerator =
            static_cast<__int128>(inverted.numerator) * integer_part + inverted.denominator;
        assert(numerator <= std::numeric_limits<long long>::max());
        return {static_cast<long long>(numerator), inverted.numerator};
    };
    return solve(solve, a, b, c, d);
}

}
