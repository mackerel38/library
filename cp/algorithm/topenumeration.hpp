#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n log n+x log x)。valuesからpicks個重複選択した多重集合和を大きい順にcount個まで返す。
template <class T>
std::vector<T> top_multiset_sums(
    std::vector<T> values,
    long long picks,
    int count
) {
    assert(picks >= 0);
    assert(count >= 0);
    if (count == 0 || values.empty()) return {};
    if (picks == 0) return {T{}};
    std::ranges::sort(values, std::greater{});

    struct state {
        T sum;
        int last;
        long long previous_count;
        long long last_count;
        bool operator<(const state& other) const {
            return sum < other.sum;
        }
    };

    std::priority_queue<state> queue;
    queue.push({values[0] * picks, 0, 0, picks});
    std::vector<T> result;
    result.reserve(count);
    while (!queue.empty() && static_cast<int>(result.size()) < count) {
        const auto [sum, last, previous_count, last_count] = queue.top();
        queue.pop();
        result.push_back(sum);
        if (previous_count > 0) {
            queue.push({
                sum - values[last - 1] + values[last],
                last,
                previous_count - 1,
                last_count + 1,
            });
        }
        if (last + 1 < static_cast<int>(values.size()) && last_count > 0) {
            queue.push({
                sum - values[last] + values[last + 1],
                last + 1,
                last_count - 1,
                1,
            });
        }
    }
    return result;
}

}
