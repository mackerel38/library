#pragma once
#include <bits/stdc++.h>
#include "math/math.hpp"

namespace poe {

/// O(log V・log V)。first+i*difference (0<=i<count) のpopcount総和を返す。最大項は2^31未満。
inline long long sum_popcount_arithmetic_progression(
    long long count,
    long long first,
    long long difference
) {
    assert(0 <= count && count < (1LL << 32));
    assert(first >= 0 && difference >= 0);
    if (count == 0) return 0;
    const __int128 maximum = static_cast<__int128>(first)
        + static_cast<__int128>(count - 1) * difference;
    assert(maximum < (1LL << 31));

    long long answer = 0;
    for (long long half_period = 1; half_period <= maximum; half_period <<= 1) {
        const long long period = half_period << 1;
        answer += floor_sum(count, period, difference, first + half_period)
            - floor_sum(count, period, difference, first);
    }
    return answer;
}

}
