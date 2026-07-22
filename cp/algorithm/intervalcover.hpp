#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"

namespace poe {

/// O(nm^2)時間・O(nm)領域。区間部分集合が[0,n)を覆う個数を選択数別に返す。
template<int mod = 998244353>
std::vector<staticmodint<mod>> interval_cover_subset_counts(
    int length,
    std::vector<std::pair<int, int>> intervals
) {
    using mint = staticmodint<mod>;
    assert(length >= 0);
    for (const auto [left, right] : intervals) {
        assert(0 <= left && left < right && right <= length);
    }
    std::sort(intervals.begin(), intervals.end());
    const int count = static_cast<int>(intervals.size());
    std::vector dp(length + 1, std::vector<mint>(count + 1));
    dp[0][0] = 1;
    for (int index = 0; index < count; ++index) {
        const auto [left, right] = intervals[index];
        auto next = dp;
        for (int covered = left; covered <= length; ++covered) {
            for (int used = 0; used <= index; ++used) {
                next[std::max(covered, right)][used + 1] += dp[covered][used];
            }
        }
        dp.swap(next);
    }
    return dp[length];
}

/// O(nm^2)時間・O(nm)領域。一様ランダムな区間で[0,n)を覆うまでの期待選択回数を返す。
template<int mod = 998244353>
staticmodint<mod> expected_random_interval_cover_time(
    int length,
    const std::vector<std::pair<int, int>>& intervals
) {
    using mint = staticmodint<mod>;
    const int count = static_cast<int>(intervals.size());
    assert(count >= 1);
    assert(count < mod);
    const auto successful = interval_cover_subset_counts<mod>(length, intervals);
    assert(successful[count] != mint{0});
    mint answer = 0;
    mint choose = 1;
    for (int used = 0; used < count; ++used) {
        const mint unsuccessful = choose - successful[used];
        answer += unsuccessful / choose * mint{count} / mint{count - used};
        choose *= mint{count - used};
        choose /= mint{used + 1};
    }
    return answer;
}

}
