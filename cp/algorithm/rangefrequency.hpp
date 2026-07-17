#pragma once
#include <bits/stdc++.h>
#include "structure/mo.hpp"

namespace poe {

/// O(k)。nCkをlong longで返す。結果がlong longに収まること。
inline long long small_binomial(long long n, int k) {
    if (k < 0 || n < k) return 0;
    k = std::min<long long>(k, n - k);
    __int128 result = 1;
    for (int i = 1; i <= k; ++i) result = result * (n - k + i) / i;
    assert(result <= std::numeric_limits<long long>::max());
    return static_cast<long long>(result);
}

/// O((n+q)sqrt(q) k+n log n)。各区間で値が等しいk添字組の個数を返す。
template <class T>
std::vector<long long> equal_tuple_range_counts(
    const std::vector<T>& values,
    const std::vector<std::pair<int, int>>& ranges,
    int tuple_size) {
    assert(tuple_size >= 0);
    std::vector<T> compressed = values;
    std::sort(compressed.begin(), compressed.end());
    compressed.erase(std::unique(compressed.begin(), compressed.end()), compressed.end());
    std::vector<int> id(values.size());
    for (int index = 0; index < static_cast<int>(values.size()); ++index) {
        id[index] = static_cast<int>(
            std::lower_bound(compressed.begin(), compressed.end(), values[index]) -
            compressed.begin());
    }

    Mo order(static_cast<int>(values.size()));
    for (const auto& [left, right] : ranges) order.add(left, right);
    std::vector<long long> result(ranges.size());
    if (tuple_size == 0) {
        std::fill(result.begin(), result.end(), 1);
        return result;
    }
    std::vector<int> frequency(compressed.size());
    long long current = 0;
    auto add = [&](int index) {
        current += small_binomial(frequency[id[index]], tuple_size - 1);
        ++frequency[id[index]];
    };
    auto remove = [&](int index) {
        --frequency[id[index]];
        current -= small_binomial(frequency[id[index]], tuple_size - 1);
    };
    order.run(add, remove, [&](int query) { result[query] = current; });
    return result;
}

}
