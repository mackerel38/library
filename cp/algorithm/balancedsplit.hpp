#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(log^2 length)。length以上ではなくthreshold以上のpieceを半分へ分割し続けた{長さ,個数}を返す。
inline std::vector<std::pair<long long, long long>> balanced_split_leaves(
    long long length,
    long long threshold
) {
    assert(length >= 1 && threshold >= 2);
    std::map<long long, long long, std::greater<>> pending{{length, 1}};
    std::map<long long, long long> leaves;
    while (!pending.empty()) {
        const auto [value, count] = *pending.begin();
        pending.erase(pending.begin());
        if (value < threshold) {
            leaves[value] += count;
            continue;
        }
        const long long lower = value / 2;
        const long long upper = value - lower;
        pending[lower] += count;
        pending[upper] += count;
    }
    return {leaves.begin(), leaves.end()};
}

/// O(n log A + n log n)。最大要素をK回半分へ分割した後のX番目に大きい値を返す。
template <std::ranges::input_range Range>
long double kth_largest_after_max_halving(
    const Range& values,
    long long operations,
    long long rank
) {
    std::vector<long double> stored;
    for (const auto& value : values) {
        assert(value > 0);
        stored.push_back(static_cast<long double>(value));
    }
    assert(!stored.empty() && operations >= 0);
    assert(1 <= rank && rank <= static_cast<long long>(stored.size()) + operations);

    const auto required = [&](long double maximum) {
        long long result = 0;
        for (long double value : stored) {
            long long pieces = 1;
            while (value > maximum) {
                value /= 2;
                pieces *= 2;
                if (pieces - 1 > operations - result) return operations + 1;
            }
            result += pieces - 1;
        }
        return result;
    };

    long double low = 0;
    long double high = *std::ranges::max_element(stored);
    for (int iteration = 0; iteration < 120; ++iteration) {
        const long double middle = std::midpoint(low, high);
        if (required(middle) <= operations) high = middle;
        else low = middle;
    }

    struct group {
        long double value;
        long long count;
    };
    std::vector<group> groups;
    long long used = 0;
    long double maximum = 0;
    for (long double value : stored) {
        long long count = 1;
        while (value > high * (1 + 1e-18L)) {
            value /= 2;
            count *= 2;
        }
        used += count - 1;
        maximum = std::max(maximum, value);
        groups.push_back({value, count});
    }
    long long remaining = operations - used;
    assert(remaining >= 0);
    std::vector<group> added;
    for (auto& current : groups) {
        if (remaining == 0) break;
        if (std::abs(current.value - maximum)
            > 1e-18L * std::max(1.0L, maximum)) continue;
        const long long split = std::min(remaining, current.count);
        current.count -= split;
        added.push_back({current.value / 2, 2 * split});
        remaining -= split;
    }
    assert(remaining == 0);
    groups.insert(groups.end(), added.begin(), added.end());

    std::ranges::sort(groups, std::greater{}, &group::value);
    for (const auto& current : groups) {
        if (rank <= current.count) return current.value;
        rank -= current.count;
    }
    std::unreachable();
}

}
