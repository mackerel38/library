#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"

namespace poe {

/// count_grouped_monotone_grid_pathsへ渡す0-indexedの行・列座標。
struct gridpathpoint {
    int row;
    int column;

    auto operator<=>(const gridpathpoint&) const = default;
};

/// O(p sqrt(hw)+p hw/sqrt(hw))時間・O(hw+p)領域。同じgroup内の二点を結ぶ右下単調path数の総和。
template <int mod>
staticmodint<mod> count_grouped_monotone_grid_paths(
    int height,
    int width,
    const std::vector<std::vector<gridpathpoint>>& groups
) {
    using mint = staticmodint<mod>;
    assert(height >= 0 && width >= 0);
    const long long area = 1LL * height * width;
    assert(height + width <= mod + 1LL);
    int point_count = 0;
    for (const auto& group : groups) {
        point_count += static_cast<int>(group.size());
        for (const auto [row, column] : group) {
            assert(0 <= row && row < height && 0 <= column && column < width);
        }
        auto sorted = group;
        std::ranges::sort(sorted);
        assert(std::ranges::adjacent_find(sorted) == sorted.end());
    }
    if (area == 0 || point_count == 0) return 0;

    const int maximum_step = height + width - 2;
    std::vector<mint> factorial(static_cast<std::size_t>(maximum_step) + 1, 1);
    std::vector<mint> inverse_factorial(static_cast<std::size_t>(maximum_step) + 1, 1);
    for (int value = 1; value <= maximum_step; ++value) {
        factorial[value] = factorial[value - 1] * value;
    }
    inverse_factorial[maximum_step] = factorial[maximum_step].inv();
    for (int value = maximum_step; value >= 1; --value) {
        inverse_factorial[value - 1] = inverse_factorial[value] * value;
    }
    const auto paths = [&](const gridpathpoint& from, const gridpathpoint& to) {
        if (from.row > to.row || from.column > to.column) return mint{};
        const int down = to.row - from.row;
        const int right = to.column - from.column;
        return factorial[down + right] * inverse_factorial[down] * inverse_factorial[right];
    };

    const int threshold = std::max(1, static_cast<int>(std::sqrt(static_cast<long double>(area))));
    mint answer = 0;
    std::vector<mint> dp(static_cast<std::size_t>(area));
    std::vector<char> marked(static_cast<std::size_t>(area));
    for (const auto& group : groups) {
        if (static_cast<int>(group.size()) <= threshold) {
            for (const auto& from : group) {
                for (const auto& to : group) answer += paths(from, to);
            }
            continue;
        }
        for (const auto point : group) {
            marked[static_cast<std::size_t>(point.row) * width + point.column] = true;
        }
        std::ranges::fill(dp, mint{});
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                const std::size_t index = static_cast<std::size_t>(row) * width + column;
                if (row > 0) dp[index] += dp[index - width];
                if (column > 0) dp[index] += dp[index - 1];
                if (marked[index]) {
                    dp[index] += 1;
                    answer += dp[index];
                }
            }
        }
        for (const auto point : group) {
            marked[static_cast<std::size_t>(point.row) * width + point.column] = false;
        }
    }
    return answer;
}

}
