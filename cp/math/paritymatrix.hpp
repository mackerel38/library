#pragma once
#include <bits/stdc++.h>
#include "math/combination.hpp"

namespace poe {

/// O(r(d+c))。column_sumsを列和とし、全成分が偶数の行がないr行の非負整数行列数を返す。
template<class mint>
mint count_matrices_without_even_rows(
    int row_count,
    const std::vector<int>& column_sums
) {
    assert(row_count >= 0);
    assert(std::ranges::all_of(column_sums, [](int value) { return value >= 0; }));
    if (row_count == 0) {
        return std::ranges::all_of(column_sums, [](int value) { return value == 0; })
            ? mint{1} : mint{};
    }

    const int maximum_sum = column_sums.empty()
        ? 0 : *std::ranges::max_element(column_sums);
    combination<mint> combinations(row_count + maximum_sum);
    std::vector<mint> coefficient(maximum_sum + 1);
    for (int sum = 0; sum <= maximum_sum; ++sum) {
        coefficient[sum] = combinations.multichoose(row_count, sum);
    }

    mint answer{};
    for (int even_rows = 0; even_rows <= row_count; ++even_rows) {
        mint ways = combinations.choose(row_count, even_rows);
        for (const int sum : column_sums) ways *= coefficient[sum];
        if (even_rows % 2 == 0) answer += ways;
        else answer -= ways;

        for (int sum = 1; sum <= maximum_sum; ++sum) {
            coefficient[sum] -= coefficient[sum - 1];
        }
    }
    return answer;
}

}
