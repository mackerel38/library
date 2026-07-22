#pragma once
#include <bits/stdc++.h>

namespace poe {
namespace detail {

template<int Prime>
std::vector<std::pair<int, long long>> adjacent_sum_skip(
    const std::vector<std::pair<int, long long>>& runs, long long distance
) {
    long long total = 0;
    for (const auto& [value, count] : runs) {
        assert(0 <= value && value < Prime && count > 0);
        total += count;
    }
    assert(0 < distance && distance < total);
    std::size_t left = 0, right = 0;
    long long left_remaining = runs[0].second;
    long long skipped = distance;
    while (skipped >= runs[right].second) skipped -= runs[right++].second;
    long long right_remaining = runs[right].second - skipped;
    long long remaining = total - distance;
    std::vector<std::pair<int, long long>> result;
    while (remaining > 0) {
        const long long take = std::min({left_remaining, right_remaining, remaining});
        const int value = (runs[left].first + runs[right].first) % Prime;
        if (!result.empty() && result.back().first == value) result.back().second += take;
        else result.emplace_back(value, take);
        remaining -= take;
        left_remaining -= take;
        right_remaining -= take;
        if (left_remaining == 0 && remaining > 0) left_remaining = runs[++left].second;
        if (right_remaining == 0 && remaining > 0) right_remaining = runs[++right].second;
    }
    return result;
}

}

/// O(各skip時のrun数の総和)時間・領域。mod Primeで隣接和による長さ1減少をsteps回行ったRLEを返す。
template<int Prime>
std::vector<std::pair<int, long long>> adjacent_sum_reduce_rle(
    std::vector<std::pair<int, long long>> runs, long long steps
) {
    static_assert(Prime >= 2);
    long long total = 0;
    for (const auto& [value, count] : runs) {
        assert(0 <= value && value < Prime && count > 0);
        total += count;
    }
    assert(0 <= steps && steps < total);
    std::vector<long long> powers{1};
    while (powers.back() <= steps / Prime) powers.push_back(powers.back() * Prime);
    for (auto iterator = powers.rbegin(); iterator != powers.rend(); ++iterator) {
        const long long power = *iterator;
        const int digit = steps / power % Prime;
        for (int repeat = 0; repeat < digit; ++repeat) {
            runs = detail::adjacent_sum_skip<Prime>(runs, power);
        }
    }
    return runs;
}

}
