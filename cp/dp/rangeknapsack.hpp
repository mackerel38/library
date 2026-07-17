#pragma once
#include <bits/stdc++.h>

namespace poe {

/// range_knapsack01へ渡す半開区間[left,right)と容量capacity。
struct rangeknapsackquery {
    int left;
    int right;
    int capacity;
};

/// O(CN log N + CQ + Q log N)時間・O(NC+Q log N)領域。各区間の0/1ナップサック最大価値を返す。
template<class T>
std::vector<T> range_knapsack01(
    const std::vector<int>& weights,
    const std::vector<T>& values,
    const std::vector<rangeknapsackquery>& queries
) {
    assert(weights.size() == values.size());
    const int size = static_cast<int>(weights.size());
    int max_capacity = 0;
    for (const auto& query : queries) {
        assert(0 <= query.left && query.left < query.right && query.right <= size);
        assert(query.capacity >= 0);
        max_capacity = std::max(max_capacity, query.capacity);
    }
    for (const int weight : weights) assert(weight >= 0);
    std::vector<T> answer(queries.size());
    if (queries.empty()) return answer;

    std::vector<std::vector<T>> dp(
        static_cast<std::size_t>(size) + 1,
        std::vector<T>(static_cast<std::size_t>(max_capacity) + 1)
    );
    auto add_item = [&](const std::vector<T>& source, std::vector<T>& target, int item) {
        target = source;
        if (weights[item] > max_capacity) return;
        for (int capacity = weights[item]; capacity <= max_capacity; ++capacity) {
            target[capacity] = std::max(
                target[capacity], source[capacity - weights[item]] + values[item]
            );
        }
    };

    auto solve = [&](auto&& self, int left, int right, const std::vector<int>& query_ids) -> void {
        if (query_ids.empty()) return;
        if (left + 1 == right) {
            for (const int id : query_ids) {
                answer[id] = weights[left] <= queries[id].capacity
                    ? std::max(T{}, values[left]) : T{};
            }
            return;
        }
        const int middle = std::midpoint(left, right);
        std::fill(dp[middle].begin(), dp[middle].end(), T{});
        for (int item = middle - 1; item >= left; --item) add_item(dp[item + 1], dp[item], item);
        for (int endpoint = middle + 1; endpoint <= right; ++endpoint) {
            add_item(dp[endpoint - 1], dp[endpoint], endpoint - 1);
        }

        std::vector<int> left_queries;
        std::vector<int> right_queries;
        left_queries.reserve(query_ids.size());
        right_queries.reserve(query_ids.size());
        for (const int id : query_ids) {
            const auto& query = queries[id];
            if (query.right <= middle) {
                left_queries.push_back(id);
            } else if (query.left >= middle) {
                right_queries.push_back(id);
            } else {
                for (int capacity = 0; capacity <= query.capacity; ++capacity) {
                    answer[id] = std::max(
                        answer[id],
                        dp[query.left][capacity] + dp[query.right][query.capacity - capacity]
                    );
                }
            }
        }
        self(self, left, middle, left_queries);
        self(self, middle, right, right_queries);
    };

    std::vector<int> query_ids(queries.size());
    std::iota(query_ids.begin(), query_ids.end(), 0);
    solve(solve, 0, size, query_ids);
    return answer;
}

}
