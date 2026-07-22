#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(3^n)時間・O(2^n)領域。集合族から互いに異なる代表元を順序付きで選ぶ方法数を返す。
template<class T, class IntersectionCount>
T count_distinct_representatives(int size, IntersectionCount intersection_count) {
    assert(0 <= size && size < std::numeric_limits<unsigned int>::digits);
    const int count = 1 << size;
    std::vector<T> block_weight(count);
    std::vector<T> factorial(size + 1, T{1});
    for (int value = 1; value <= size; ++value) factorial[value] = factorial[value - 1] * value;
    for (int mask = 1; mask < count; ++mask) {
        const int elements = std::popcount(static_cast<unsigned int>(mask));
        block_weight[mask] = T{intersection_count(mask)} * factorial[elements - 1];
        if (elements % 2 == 0) block_weight[mask] = -block_weight[mask];
    }

    std::vector<T> dp(count);
    dp[0] = T{1};
    for (int mask = 1; mask < count; ++mask) {
        const int first = mask & -mask;
        for (int block = mask; block > 0; block = (block - 1) & mask) {
            if (block & first) dp[mask] += block_weight[block] * dp[mask ^ block];
        }
    }
    return dp.back();
}

/// O(3^n)時間・O(2^n)領域。各iがdivisors[i]の倍数を選び、全値が相異なる列の個数を返す。
template<class T>
T count_distinct_bounded_multiples(
    long long limit,
    const std::vector<long long>& divisors
) {
    assert(limit >= 1);
    assert(divisors.size() < std::numeric_limits<unsigned int>::digits);
    const int size = static_cast<int>(divisors.size());
    const int count = 1 << size;
    std::vector<long long> least_common_multiple(count);
    least_common_multiple[0] = 1;
    for (int mask = 1; mask < count; ++mask) {
        const int bit = std::countr_zero(static_cast<unsigned int>(mask));
        const int previous = mask & (mask - 1);
        const long long divisor = divisors[bit];
        assert(1 <= divisor && divisor <= limit);
        if (least_common_multiple[previous] == 0) continue;
        const long long reduced = least_common_multiple[previous]
                                / std::gcd(least_common_multiple[previous], divisor);
        if (reduced > limit / divisor) continue;
        least_common_multiple[mask] = reduced * divisor;
    }
    return count_distinct_representatives<T>(size, [&](int mask) {
        return least_common_multiple[mask] == 0
             ? 0LL : limit / least_common_multiple[mask];
    });
}

}
