#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"

namespace poe {

/// O(hw2^min(h,w))。行左prefixと列上prefixのORで作れpatternに一致するgrid数を返す。
template<int mod>
staticmodint<mod> count_row_column_prefix_union(std::vector<std::string> pattern) {
    assert(!pattern.empty() && !pattern[0].empty());
    int height = static_cast<int>(pattern.size());
    int width = static_cast<int>(pattern[0].size());
    for (const std::string& row : pattern) {
        assert(static_cast<int>(row.size()) == width);
        for (const char cell : row) assert(cell == '0' || cell == '1' || cell == '?');
    }
    if (width > height) {
        std::vector<std::string> transposed(width, std::string(height, '?'));
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                transposed[column][row] = pattern[row][column];
            }
        }
        pattern = std::move(transposed);
        std::swap(height, width);
    }
    assert(width < 31);
    using mint = staticmodint<mod>;
    const int states = 1 << width;
    std::vector<mint> dp(states);
    dp[states - 1] = 1;
    for (int row = 0; row < height; ++row) {
        std::vector<mint> next = dp;
        bool prefix_can_be_one = true;
        for (int column = 0; column < width; ++column) {
            std::vector<mint> work(states);
            if (pattern[row][column] != '1') {
                for (int mask = 0; mask < states; ++mask) {
                    const int destination = mask & ~(1 << column);
                    work[destination] += next[mask];
                }
            }
            if (pattern[row][column] != '0') {
                for (int mask = 0; mask < states; ++mask) {
                    if (mask >> column & 1) work[mask] += next[mask];
                }
            }
            next = std::move(work);
            prefix_can_be_one &= pattern[row][column] != '0';
            if (!prefix_can_be_one) continue;
            for (int mask = 0; mask < states; ++mask) {
                if (!(mask >> column & 1)) next[mask] += dp[mask];
            }
        }
        dp = std::move(next);
    }
    return std::accumulate(dp.begin(), dp.end(), mint{});
}

}
