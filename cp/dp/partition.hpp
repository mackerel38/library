#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 列を各フェーズ一個以上の連続区間へ分け最大得点を返す: max_phase_partition(score); O(pn)。
template <class T>
T max_phase_partition(const std::vector<std::vector<T>>& score) {
    assert(!score.empty());
    const int phases = static_cast<int>(score.size());
    const int size = static_cast<int>(score[0].size());
    assert(size >= phases);
    for (const auto& row : score) assert(static_cast<int>(row.size()) == size);

    std::vector<T> dp(size);
    dp[0] = score[0][0];
    for (int index = 1; index < size; ++index) dp[index] = dp[index - 1] + score[0][index];
    for (int phase = 1; phase < phases; ++phase) {
        std::vector<T> next(size);
        next[phase] = dp[phase - 1] + score[phase][phase];
        for (int index = phase + 1; index < size; ++index) {
            next[index] = std::max(next[index - 1], dp[index - 1]) + score[phase][index];
        }
        dp.swap(next);
    }
    return dp.back();
}

/// O(n)時間・O(n)領域。全連続分割について各区間の(max-min)を掛けた値の総和を返す。
template <class T, class Value>
T sum_partition_range_products(const std::vector<Value>& values) {
    std::vector<T> dp(values.size() + 1);
    dp[0] = T{1};
    std::vector<std::pair<Value, T>> maximum_stack, minimum_stack;
    T maximum_sum{}, minimum_sum{};
    for (int index = 0; index < static_cast<int>(values.size()); ++index) {
        T maximum_weight = dp[index];
        while (!maximum_stack.empty() && !(values[index] < maximum_stack.back().first)) {
            maximum_sum -= T{maximum_stack.back().first} * maximum_stack.back().second;
            maximum_weight += maximum_stack.back().second;
            maximum_stack.pop_back();
        }
        maximum_stack.emplace_back(values[index], maximum_weight);
        maximum_sum += T{values[index]} * maximum_weight;

        T minimum_weight = dp[index];
        while (!minimum_stack.empty() && !(minimum_stack.back().first < values[index])) {
            minimum_sum -= T{minimum_stack.back().first} * minimum_stack.back().second;
            minimum_weight += minimum_stack.back().second;
            minimum_stack.pop_back();
        }
        minimum_stack.emplace_back(values[index], minimum_weight);
        minimum_sum += T{values[index]} * minimum_weight;
        dp[index + 1] = maximum_sum - minimum_sum;
    }
    return dp.back();
}

/// O(n^3 s^2)。二項演算を全ての括弧付けで適用して到達できる状態をbit集合で返す。s<=64。
inline unsigned long long parenthesized_results(
    const std::vector<int>& values,
    const std::vector<std::vector<int>>& operation
) {
    assert(!values.empty() && !operation.empty() && operation.size() <= 64);
    const int size = static_cast<int>(values.size());
    const int states = static_cast<int>(operation.size());
    for (const auto& row : operation) assert(static_cast<int>(row.size()) == states);
    for (const auto& row : operation) {
        for (const int result : row) assert(0 <= result && result < states);
    }
    for (const int value : values) assert(0 <= value && value < states);

    std::vector dp(size, std::vector<unsigned long long>(size + 1));
    for (int index = 0; index < size; ++index) dp[index][index + 1] = 1ULL << values[index];
    for (int length = 2; length <= size; ++length) {
        for (int left = 0; left + length <= size; ++left) {
            const int right = left + length;
            for (int middle = left + 1; middle < right; ++middle) {
                auto left_mask = dp[left][middle];
                while (left_mask) {
                    const int left_state = std::countr_zero(left_mask);
                    left_mask &= left_mask - 1;
                    auto right_mask = dp[middle][right];
                    while (right_mask) {
                        const int right_state = std::countr_zero(right_mask);
                        right_mask &= right_mask - 1;
                        dp[left][right] |= 1ULL << operation[left_state][right_state];
                    }
                }
            }
        }
    }
    return dp[0][size];
}

}
