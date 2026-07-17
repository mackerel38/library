#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n 2^n)時間・O(2^n)空間。同じ残存列への削除を重複させず、全削除順の個数を返す。
template <class T, std::ranges::input_range Range>
T count_distinct_deletion_orders(const Range& sequence) {
    using value_type = std::ranges::range_value_t<Range>;
    const std::vector<value_type> values(
        std::ranges::begin(sequence), std::ranges::end(sequence));
    const int n = static_cast<int>(values.size());
    assert(n < std::numeric_limits<std::size_t>::digits);
    const std::size_t count = std::size_t{1} << n;
    std::vector<T> dp(count);
    dp.back() = T{1};
    for (std::size_t mask = count; mask-- > 0;) {
        int previous = -1;
        for (int index = 0; index < n; ++index) {
            if ((mask >> index & 1U) == 0) continue;
            if (previous == -1 || values[previous] != values[index]) {
                dp[mask ^ (std::size_t{1} << index)] += dp[mask];
            }
            previous = index;
        }
    }
    return dp[0];
}

/// 集合分割DPの結果。costが最小費用、groupsが選んだ非空部分集合のbit mask列。
template<class T>
struct subsetpartitionresult {
    T cost;
    std::vector<std::uint64_t> groups;
};

/// 零和集合分割の結果。groupsは和が0の非空部分集合bit mask列で、個数が最大。
struct zerosumpartitionresult {
    std::vector<std::uint64_t> groups;
};

/// O(n 2^n)時間・O(2^n)空間。全体和が0なら零和部分集合への最大個数分割を復元する。
template <class T>
zerosumpartitionresult max_zero_sum_partition(const std::vector<T>& values) {
    const int n = static_cast<int>(values.size());
    assert(0 <= n && n < 63);
    const std::uint64_t count = std::uint64_t{1} << n;
    std::vector<T> sums(static_cast<std::size_t>(count));
    std::vector<int> best(static_cast<std::size_t>(count), -1);
    std::vector<int> previous(static_cast<std::size_t>(count), -1);
    best[0] = 0;
    for (std::uint64_t mask = 1; mask < count; ++mask) {
        const int bit = std::countr_zero(mask);
        sums[mask] = sums[mask ^ (std::uint64_t{1} << bit)] + values[bit];
        for (std::uint64_t remaining = mask; remaining != 0; remaining &= remaining - 1) {
            const int last = std::countr_zero(remaining);
            const std::uint64_t before = mask ^ (std::uint64_t{1} << last);
            const int candidate = best[before] + (sums[mask] == T{});
            if (candidate > best[mask]) {
                best[mask] = candidate;
                previous[mask] = last;
            }
        }
    }
    if (sums[count - 1] != T{}) return {};

    std::vector<int> order;
    for (std::uint64_t mask = count - 1; mask != 0;) {
        const int last = previous[mask];
        assert(last != -1);
        order.push_back(last);
        mask ^= std::uint64_t{1} << last;
    }
    std::ranges::reverse(order);

    zerosumpartitionresult result;
    std::uint64_t group = 0;
    T sum{};
    for (const int index : order) {
        group |= std::uint64_t{1} << index;
        sum += values[index];
        if (sum == T{}) {
            result.groups.push_back(group);
            group = 0;
        }
    }
    assert(group == 0);
    return result;
}

/// O(3^n)時間・O(2^n)空間。各非空部分集合の費用から、全要素を分割する最小費用と分割を返す。
template<class T>
subsetpartitionresult<T> subset_partition_min(
    int n,
    const std::vector<T>& group_cost,
    const T& inf
) {
    assert(0 <= n && n < 63);
    const std::uint64_t count = std::uint64_t{1} << n;
    assert(group_cost.size() == count);
    std::vector<T> dp(static_cast<std::size_t>(count), inf);
    std::vector<std::uint64_t> choice(static_cast<std::size_t>(count));
    dp[0] = T{};
    for (std::uint64_t mask = 1; mask < count; ++mask) {
        const std::uint64_t anchor = mask & -mask;
        for (std::uint64_t group = mask; group != 0; group = (group - 1) & mask) {
            if ((group & anchor) == 0 || group_cost[group] == inf) continue;
            const std::uint64_t remaining = mask ^ group;
            if (dp[remaining] == inf) continue;
            const T candidate = dp[remaining] + group_cost[group];
            if (candidate < dp[mask]) {
                dp[mask] = candidate;
                choice[mask] = group;
            }
        }
    }

    subsetpartitionresult<T> result{dp[count - 1], {}};
    if (result.cost == inf) return result;
    for (std::uint64_t mask = count - 1; mask != 0;) {
        const std::uint64_t group = choice[mask];
        assert(group != 0);
        result.groups.push_back(group);
        mask ^= group;
    }
    return result;
}

}
