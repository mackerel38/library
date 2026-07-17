#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(rows * columns * log columns)。divide_conquer_dpで各行の最小遷移を単調性付きで計算する。
/// cost(row, previous, column)を返し、最適previousがcolumnに対して広義単調増加であること。
template <class T, class Cost>
std::vector<T> divide_conquer_dp(
    int rows,
    int columns,
    std::vector<T> initial,
    Cost cost,
    T inf
) {
    assert(rows >= 0 && columns >= 0);
    assert(static_cast<int>(initial.size()) == columns);
    std::vector<T> previous = std::move(initial), current(columns, inf);
    auto solve = [&](auto&& self, int row, int left, int right, int opt_left, int opt_right) -> void {
        if (left >= right) return;
        const int middle = (left + right) / 2;
        std::pair<T, int> best{inf, -1};
        const int last = std::min(middle, opt_right);
        for (int before = opt_left; before <= last; ++before) {
            if (previous[before] == inf) continue;
            best = std::min(best, std::pair<T, int>{
                previous[before] + cost(row, before, middle), before
            });
        }
        current[middle] = best.first;
        const int split = best.second == -1 ? opt_left : best.second;
        self(self, row, left, middle, opt_left, split);
        self(self, row, middle + 1, right, split, opt_right);
    };
    for (int row = 0; row < rows; ++row) {
        std::fill(current.begin(), current.end(), inf);
        if (columns > 0) solve(solve, row, 0, columns, 0, columns - 1);
        previous.swap(current);
    }
    return previous;
}

/// O(n^2)。optimal_merge_cost(weights): 隣接する二群を総和コストで併合する最小費用を返す。
/// weightsは非負であること。Knuth最適化を用いる。
template <class T>
T optimal_merge_cost(const std::vector<T>& weights) {
    const int n = static_cast<int>(weights.size());
    if (n <= 1) return T{};
    std::vector<T> prefix(static_cast<std::size_t>(n) + 1);
    for (int index = 0; index < n; ++index) {
        assert(!(weights[index] < T{}));
        prefix[index + 1] = prefix[index] + weights[index];
    }
    std::vector dp(n + 1, std::vector<T>(n + 1));
    std::vector opt(n + 1, std::vector<int>(n + 1));
    for (int left = 0; left < n; ++left) opt[left][left + 1] = left + 1;
    for (int length = 2; length <= n; ++length) {
        for (int left = 0; left + length <= n; ++left) {
            const int right = left + length;
            const int begin = std::max(left + 1, opt[left][right - 1]);
            const int end = std::min(right - 1, opt[left + 1][right]);
            std::optional<T> best;
            for (int middle = begin; middle <= end; ++middle) {
                const T candidate = dp[left][middle] + dp[middle][right];
                if (!best || candidate < *best) {
                    best = candidate;
                    opt[left][right] = middle;
                }
            }
            assert(best.has_value());
            dp[left][right] = *best + prefix[right] - prefix[left];
        }
    }
    return dp[0][n];
}

}
