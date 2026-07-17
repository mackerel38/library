#pragma once
#include <bits/stdc++.h>

namespace poe {

/// right_transfer_isotonicの結果。valuesは操作後の広義単調増加列、operationsは最小操作回数。
struct righttransferisotonicresult {
    std::vector<long long> values;
    long long operations;
};

namespace isotonic_detail {

inline long long floor_div(long long numerator, long long denominator) {
    assert(denominator > 0);
    long long quotient = numerator / denominator;
    const long long remainder = numerator % denominator;
    if (remainder < 0) --quotient;
    return quotient;
}

inline long long ceil_div(long long numerator, long long denominator) {
    return -floor_div(-numerator, denominator);
}

}

/// O(n)。各操作でvalues[i]を1減らしvalues[i+1]を1増やして広義単調増加にする最小操作を求める。
inline righttransferisotonicresult right_transfer_isotonic(const std::vector<long long>& values) {
    struct block {
        int length;
        long long sum;
    };

    std::vector<block> blocks;
    blocks.reserve(values.size());
    for (const long long value : values) {
        block current{1, value};
        while (!blocks.empty() &&
               isotonic_detail::ceil_div(blocks.back().sum, blocks.back().length) >
                   isotonic_detail::floor_div(current.sum, current.length)) {
            current.length += blocks.back().length;
            current.sum += blocks.back().sum;
            blocks.pop_back();
        }
        blocks.push_back(current);
    }

    righttransferisotonicresult result;
    result.values.reserve(values.size());
    for (const auto [length, sum] : blocks) {
        for (int i = 0; i < length; ++i) {
            result.values.push_back(isotonic_detail::floor_div(sum + i, length));
        }
    }
    result.operations = 0;
    for (int i = 0; i < static_cast<int>(values.size()); ++i) {
        result.operations += 1LL * i * (result.values[i] - values[i]);
    }
    return result;
}

}
