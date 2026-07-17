#pragma once
#include <bits/stdc++.h>

namespace poe {

/// offline_range_min_sumへ渡す半開区間と上限値。
template <class T>
struct rangeminsumquery {
    int left;
    int right;
    T cap;
};

/// 各queryのsum(min(values[i], cap))を一括計算する: offline_range_min_sum(values, queries); O((n+q)log n)。
template <class T>
std::vector<T> offline_range_min_sum(const std::vector<T>& values,
                                     const std::vector<rangeminsumquery<T>>& queries) {
    const int size = static_cast<int>(values.size());
    std::vector<int> positions(size);
    std::iota(positions.begin(), positions.end(), 0);
    std::ranges::sort(positions, {}, [&](int index) -> const T& { return values[index]; });

    std::vector<int> order(queries.size());
    std::iota(order.begin(), order.end(), 0);
    std::ranges::sort(order, {}, [&](int index) -> const T& { return queries[index].cap; });

    std::vector<long long> count(size + 1);
    std::vector<T> sum(size + 1);
    auto add = [&](int index, long long count_delta, const T& sum_delta) {
        for (++index; index <= size; index += index & -index) {
            count[index] += count_delta;
            sum[index] += sum_delta;
        }
    };
    auto prefix_count = [&](int right) {
        long long result = 0;
        for (; right > 0; right -= right & -right) result += count[right];
        return result;
    };
    auto prefix_sum = [&](int right) {
        T result{};
        for (; right > 0; right -= right & -right) result += sum[right];
        return result;
    };

    std::vector<T> answer(queries.size());
    int activated = 0;
    for (int query_index : order) {
        const auto& query = queries[query_index];
        assert(0 <= query.left && query.left <= query.right && query.right <= size);
        while (activated < size && !(query.cap < values[positions[activated]])) {
            const int position = positions[activated++];
            add(position, 1, values[position]);
        }
        const long long low_count = prefix_count(query.right) - prefix_count(query.left);
        const T low_sum = prefix_sum(query.right) - prefix_sum(query.left);
        answer[query_index] = low_sum +
            query.cap * (static_cast<long long>(query.right - query.left) - low_count);
    }
    return answer;
}

}
