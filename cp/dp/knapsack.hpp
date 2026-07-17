#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(nW)。knapsack01(weight, value, W): 重さW以下の0/1ナップサック最大価値を返す。
template <class T>
T knapsack01(
    const std::vector<int>& weights,
    const std::vector<T>& values,
    int capacity
) {
    assert(weights.size() == values.size());
    assert(capacity >= 0);
    const T unreachable = std::numeric_limits<T>::lowest();
    std::vector<T> dp(static_cast<std::size_t>(capacity) + 1, unreachable);
    dp[0] = T{};
    for (std::size_t item = 0; item < weights.size(); ++item) {
        assert(weights[item] >= 0);
        if (weights[item] > capacity) continue;
        for (int weight = capacity; weight >= weights[item]; --weight) {
            if (dp[weight - weights[item]] == unreachable) continue;
            dp[weight] = std::max(dp[weight], dp[weight - weights[item]] + values[item]);
        }
    }
    return *std::max_element(dp.begin(), dp.end());
}

/// 最適な0/1ナップサック解における品物の使われ方。
enum class knapsackitemstatus {
    mandatory,
    optional,
    excluded,
};

/// O(nW)時間・O(nW)領域。全最適解で各品物が必須・任意・不使用かを返す。
template <class T>
std::vector<knapsackitemstatus> classify_knapsack01_items(
    const std::vector<int>& weights,
    const std::vector<T>& values,
    int capacity
) {
    assert(weights.size() == values.size());
    assert(capacity >= 0);
    const int n = static_cast<int>(weights.size());
    std::vector<std::vector<T>> prefix(
        static_cast<std::size_t>(n) + 1,
        std::vector<T>(static_cast<std::size_t>(capacity) + 1, T{})
    );
    for (int item = 0; item < n; ++item) {
        assert(weights[item] >= 0);
        for (int weight = 0; weight <= capacity; ++weight) {
            prefix[item + 1][weight] = prefix[item][weight];
            if (weights[item] <= weight) {
                prefix[item + 1][weight] = std::max(
                    prefix[item + 1][weight],
                    prefix[item][weight - weights[item]] + values[item]
                );
            }
        }
    }

    const T optimum = prefix[n][capacity];
    std::vector<T> suffix(static_cast<std::size_t>(capacity) + 1, T{});
    std::vector<knapsackitemstatus> result(n);
    for (int item = n - 1; item >= 0; --item) {
        T best_excluding{};
        T best_beside_included{};
        const int included_capacity = capacity - weights[item];
        for (int left_weight = 0; left_weight <= capacity; ++left_weight) {
            best_excluding = std::max(
                best_excluding,
                prefix[item][left_weight] + suffix[capacity - left_weight]
            );
            if (left_weight <= included_capacity) {
                best_beside_included = std::max(
                    best_beside_included,
                    prefix[item][left_weight] + suffix[included_capacity - left_weight]
                );
            }
        }
        const bool can_exclude = best_excluding == optimum;
        const bool can_include = weights[item] <= capacity
            && best_beside_included + values[item] == optimum;
        if (can_exclude && can_include) {
            result[item] = knapsackitemstatus::optional;
        } else if (can_include) {
            result[item] = knapsackitemstatus::mandatory;
        } else {
            result[item] = knapsackitemstatus::excluded;
        }

        if (weights[item] <= capacity) {
            for (int weight = capacity; weight >= weights[item]; --weight) {
                suffix[weight] = std::max(
                    suffix[weight], suffix[weight - weights[item]] + values[item]
                );
            }
        }
    }
    return result;
}

/// O(nV)。knapsack01_by_value(weight, value): 各価値を作る最小重量を返す。価値は非負整数。
inline std::vector<long long> knapsack01_by_value(
    const std::vector<long long>& weights,
    const std::vector<int>& values
) {
    assert(weights.size() == values.size());
    int value_sum = 0;
    for (const int value : values) {
        assert(value >= 0);
        assert(value_sum <= std::numeric_limits<int>::max() - value);
        value_sum += value;
    }
    const long long inf = std::numeric_limits<long long>::max() / 4;
    std::vector<long long> dp(static_cast<std::size_t>(value_sum) + 1, inf);
    dp[0] = 0;
    int current_sum = 0;
    for (std::size_t item = 0; item < weights.size(); ++item) {
        assert(weights[item] >= 0);
        for (int value = current_sum; value >= 0; --value) {
            if (dp[value] == inf) continue;
            dp[value + values[item]] = std::min(
                dp[value + values[item]], dp[value] + weights[item]
            );
        }
        current_sum += values[item];
    }
    return dp;
}

/// O(nV)。knapsack01_value(weight, value, W): 価値DPで重さW以下の最大価値を返す。
inline long long knapsack01_value(
    const std::vector<long long>& weights,
    const std::vector<int>& values,
    long long capacity
) {
    assert(capacity >= 0);
    const auto minimum_weight = knapsack01_by_value(weights, values);
    long long answer = 0;
    for (int value = 0; value < static_cast<int>(minimum_weight.size()); ++value) {
        if (minimum_weight[value] <= capacity) answer = value;
    }
    return answer;
}

namespace detail {

template<class T>
std::vector<std::pair<long long, T>> knapsack_pareto(
    const std::vector<long long>& weights,
    const std::vector<T>& values,
    int left,
    int right,
    long long capacity
) {
    std::vector<std::pair<long long, T>> states{{0, T{}}};
    for (int item = left; item < right; ++item) {
        std::vector<std::pair<long long, T>> selected;
        selected.reserve(states.size());
        for (const auto& [weight, value] : states) {
            if (weights[item] <= capacity - weight) {
                selected.emplace_back(weight + weights[item], value + values[item]);
            }
        }
        std::vector<std::pair<long long, T>> merged;
        merged.reserve(states.size() + selected.size());
        std::size_t first = 0, second = 0;
        std::optional<T> best;
        while (first < states.size() || second < selected.size()) {
            const long long weight = second == selected.size()
                || (first < states.size() && states[first].first <= selected[second].first)
                ? states[first].first : selected[second].first;
            std::optional<T> value;
            while (first < states.size() && states[first].first == weight) {
                if (!value || *value < states[first].second) value = states[first].second;
                ++first;
            }
            while (second < selected.size() && selected[second].first == weight) {
                if (!value || *value < selected[second].second) value = selected[second].second;
                ++second;
            }
            if (!best || *best < *value) {
                best = *value;
                merged.emplace_back(weight, *value);
            }
        }
        states = std::move(merged);
    }
    return states;
}

inline unsigned long long knapsack_cost(unsigned long long first, unsigned long long second) {
    constexpr auto inf = std::numeric_limits<unsigned long long>::max();
    if (first != 0 && second > inf / first) return inf;
    return first * second;
}

}

/// O(2^(n/2))時間・領域。knapsack01_mitm(weight, value, W): 半分全列挙で最大価値を返す。
template<class T>
T knapsack01_mitm(
    const std::vector<long long>& weights,
    const std::vector<T>& values,
    long long capacity
) {
    assert(weights.size() == values.size());
    assert(capacity >= 0);
    assert(weights.size() <= 62);
    for (const long long weight : weights) assert(weight >= 0);
    const int middle = static_cast<int>(weights.size() / 2);
    auto left = detail::knapsack_pareto(weights, values, 0, middle, capacity);
    auto right = detail::knapsack_pareto(
        weights, values, middle, static_cast<int>(weights.size()), capacity
    );
    T answer{};
    int right_index = static_cast<int>(right.size()) - 1;
    for (const auto& [weight, value] : left) {
        while (right_index >= 0 && right[right_index].first > capacity - weight) --right_index;
        if (right_index >= 0) answer = std::max(answer, value + right[right_index].second);
    }
    return answer;
}

/// 容量DP・価値DP・半分全列挙の最小推定計算量。knapsack01_auto(weight, value, W): 最大価値を返す。
/// 価値は非負整数で、いずれかの解法の状態数がメモリに載る制約で使う。
inline long long knapsack01_auto(
    const std::vector<long long>& weights,
    const std::vector<long long>& values,
    long long capacity
) {
    assert(weights.size() == values.size());
    assert(capacity >= 0);
    const unsigned long long n = weights.size();
    unsigned long long weight_sum = 0;
    unsigned long long value_sum = 0;
    for (std::size_t item = 0; item < weights.size(); ++item) {
        assert(weights[item] >= 0 && values[item] >= 0);
        const auto weight = static_cast<unsigned long long>(weights[item]);
        const auto value = static_cast<unsigned long long>(values[item]);
        weight_sum = std::min(
            std::numeric_limits<unsigned long long>::max() - weight_sum, weight
        ) == weight ? weight_sum + weight : std::numeric_limits<unsigned long long>::max();
        value_sum = std::min(
            std::numeric_limits<unsigned long long>::max() - value_sum, value
        ) == value ? value_sum + value : std::numeric_limits<unsigned long long>::max();
    }
    const auto effective_capacity = std::min<unsigned long long>(capacity, weight_sum);
    const auto weight_cost = effective_capacity < static_cast<unsigned long long>(std::numeric_limits<int>::max())
        ? detail::knapsack_cost(n, effective_capacity + 1)
        : std::numeric_limits<unsigned long long>::max();
    const auto value_cost = value_sum <= static_cast<unsigned long long>(std::numeric_limits<int>::max())
        ? detail::knapsack_cost(n, value_sum + 1)
        : std::numeric_limits<unsigned long long>::max();
    const auto mitm_cost = n <= 62
        ? (1ULL << ((n + 1) / 2))
        : std::numeric_limits<unsigned long long>::max();

    if (weight_cost <= value_cost && weight_cost <= mitm_cost) {
        std::vector<int> converted(weights.size());
        for (std::size_t item = 0; item < weights.size(); ++item) {
            converted[item] = static_cast<int>(std::min<unsigned long long>(weights[item], effective_capacity + 1));
        }
        return knapsack01(converted, values, static_cast<int>(effective_capacity));
    }
    if (value_cost <= mitm_cost) {
        std::vector<int> converted(values.size());
        for (std::size_t item = 0; item < values.size(); ++item) {
            converted[item] = static_cast<int>(values[item]);
        }
        return knapsack01_value(weights, converted, capacity);
    }
    assert(mitm_cost != std::numeric_limits<unsigned long long>::max());
    return knapsack01_mitm(weights, values, capacity);
}

/// O(nW)。unbounded_knapsack(weight, value, W): 同じ品物を何個でも使える最大価値を返す。
template <class T>
T unbounded_knapsack(
    const std::vector<int>& weights,
    const std::vector<T>& values,
    int capacity
) {
    assert(weights.size() == values.size());
    assert(capacity >= 0);
    std::vector<T> dp(static_cast<std::size_t>(capacity) + 1, T{});
    for (std::size_t item = 0; item < weights.size(); ++item) {
        assert(weights[item] > 0);
        for (int weight = weights[item]; weight <= capacity; ++weight) {
            dp[weight] = std::max(dp[weight], dp[weight - weights[item]] + values[item]);
        }
    }
    return dp[capacity];
}

/// O(nC^2)時間・O(C^2)領域。小さい正コスト品を無制限に選び、価値target以上の最小コストを返す。
/// Cは最大コスト。valuesとtargetは正で、答えがlong longに収まること。
inline long long minimum_unbounded_knapsack_cost(
    const std::vector<int>& costs,
    const std::vector<long long>& values,
    long long target
) {
    assert(costs.size() == values.size());
    assert(!costs.empty() && target > 0);
    int best = 0;
    int maximum_cost = 0;
    for (int item = 0; item < static_cast<int>(costs.size()); ++item) {
        assert(costs[item] > 0 && values[item] > 0);
        maximum_cost = std::max(maximum_cost, costs[item]);
        if (
            static_cast<__int128>(values[item]) * costs[best]
            > static_cast<__int128>(values[best]) * costs[item]
        ) {
            best = item;
        }
    }
    const long long residual_limit_wide =
        static_cast<long long>(costs[best]) * maximum_cost;
    assert(residual_limit_wide <= std::numeric_limits<int>::max());
    const int residual_limit = static_cast<int>(residual_limit_wide);
    constexpr long long unreachable = -1;
    std::vector<long long> dp(static_cast<std::size_t>(residual_limit) + 1, unreachable);
    dp[0] = 0;
    for (int item = 0; item < static_cast<int>(costs.size()); ++item) {
        for (int cost = costs[item]; cost <= residual_limit; ++cost) {
            if (dp[cost - costs[item]] == unreachable) continue;
            const long long candidate = values[item] >= target - dp[cost - costs[item]]
                ? target : dp[cost - costs[item]] + values[item];
            dp[cost] = std::max(dp[cost], candidate);
        }
    }
    __int128 answer = std::numeric_limits<long long>::max();
    for (int cost = 0; cost <= residual_limit; ++cost) {
        if (dp[cost] == unreachable) continue;
        const long long missing = target - dp[cost];
        const long long copies = missing <= 0
            ? 0 : (missing - 1) / values[best] + 1;
        answer = std::min(
            answer,
            static_cast<__int128>(cost) + static_cast<__int128>(copies) * costs[best]
        );
    }
    assert(answer <= std::numeric_limits<long long>::max());
    return static_cast<long long>(answer);
}

/// O(nW)。bounded_knapsack(weight, value, count, W): 各品物をcount個まで使える最大価値を返す。
template<class T>
T bounded_knapsack(
    const std::vector<int>& weights,
    const std::vector<T>& values,
    const std::vector<long long>& counts,
    int capacity
) {
    assert(weights.size() == values.size() && values.size() == counts.size());
    assert(capacity >= 0);
    std::vector<T> dp(static_cast<std::size_t>(capacity) + 1, T{}), next(dp.size());
    for (std::size_t item = 0; item < weights.size(); ++item) {
        assert(weights[item] > 0 && counts[item] >= 0);
        const int count = static_cast<int>(std::min<long long>(counts[item], capacity / weights[item]));
        next = dp;
        for (int remainder = 0; remainder < weights[item] && remainder <= capacity; ++remainder) {
            std::deque<int> candidates;
            const int length = (capacity - remainder) / weights[item];
            auto score = [&](int index) {
                return dp[remainder + index * weights[item]] - values[item] * index;
            };
            for (int index = 0; index <= length; ++index) {
                while (!candidates.empty() && candidates.front() < index - count) candidates.pop_front();
                while (!candidates.empty() && score(candidates.back()) <= score(index)) candidates.pop_back();
                candidates.push_back(index);
                next[remainder + index * weights[item]] =
                    score(candidates.front()) + values[item] * index;
            }
        }
        dp.swap(next);
    }
    return dp[capacity];
}

namespace detail {

template <class T>
struct tinyweightgroupresult {
    bool valid;
    T fixed_value;
    std::vector<std::pair<T, long long>> groups;
};

template <class T>
tinyweightgroupresult<T> group_tiny_weight_items(
    std::vector<std::pair<T, long long>> items,
    int fixed_count,
    int group_size
) {
    T fixed_value{};
    int remaining_fixed = fixed_count;
    while (remaining_fixed > 0 && !items.empty()) {
        auto& [value, count] = items.back();
        const long long take = std::min<long long>(remaining_fixed, count);
        fixed_value += value * take;
        count -= take;
        remaining_fixed -= static_cast<int>(take);
        if (count == 0) items.pop_back();
    }
    if (remaining_fixed > 0) return {false, T{}, {}};

    std::vector<std::pair<T, long long>> groups;
    long long partial_count = 0;
    T partial_value{};
    while (!items.empty()) {
        auto [value, count] = items.back();
        items.pop_back();
        if (partial_count > 0) {
            const long long take = std::min<long long>(group_size - partial_count, count);
            partial_count += take;
            partial_value += value * take;
            count -= take;
            if (partial_count == group_size) {
                groups.emplace_back(partial_value, 1);
                partial_count = 0;
                partial_value = T{};
            }
            if (count == 0 && partial_count > 0) continue;
        }
        if (count >= group_size) {
            groups.emplace_back(value * group_size, count / group_size);
            count %= group_size;
        }
        partial_count = count;
        partial_value = value * count;
    }
    return {true, fixed_value, std::move(groups)};
}

}

/// O(n log n)。重みが1,2,3だけの個数制限knapsackを巨大capacityでも解く。
template <class T>
T bounded_knapsack_tiny_weights(
    const std::vector<int>& weights,
    const std::vector<T>& values,
    const std::vector<long long>& counts,
    long long capacity
) {
    assert(weights.size() == values.size() && values.size() == counts.size());
    assert(capacity >= 0);
    std::array<std::vector<std::pair<T, long long>>, 4> items;
    for (std::size_t item = 0; item < weights.size(); ++item) {
        assert(1 <= weights[item] && weights[item] <= 3);
        assert(values[item] >= T{} && counts[item] >= 0);
        if (counts[item] > 0) items[weights[item]].emplace_back(values[item], counts[item]);
    }
    for (int weight = 1; weight <= 3; ++weight) std::ranges::sort(items[weight]);

    T answer{};
    for (int remainder1 = 0; remainder1 < 6; ++remainder1) {
        const auto group1 = detail::group_tiny_weight_items(items[1], remainder1, 6);
        if (!group1.valid) continue;
        for (int remainder2 = 0; remainder2 < 3; ++remainder2) {
            const auto group2 = detail::group_tiny_weight_items(items[2], remainder2, 3);
            if (!group2.valid) continue;
            for (int remainder3 = 0; remainder3 < 2; ++remainder3) {
                const auto group3 = detail::group_tiny_weight_items(items[3], remainder3, 2);
                if (!group3.valid) continue;
                const long long fixed_weight = remainder1 + 2LL * remainder2 + 3LL * remainder3;
                if (fixed_weight > capacity) continue;
                long long remaining_groups = (capacity - fixed_weight) / 6;
                T current = group1.fixed_value + group2.fixed_value + group3.fixed_value;
                std::vector<std::pair<T, long long>> groups = group1.groups;
                groups.insert(groups.end(), group2.groups.begin(), group2.groups.end());
                groups.insert(groups.end(), group3.groups.begin(), group3.groups.end());
                std::ranges::sort(groups);
                while (remaining_groups > 0 && !groups.empty()) {
                    const auto [value, count] = groups.back();
                    groups.pop_back();
                    const long long take = std::min(remaining_groups, count);
                    current += value * take;
                    remaining_groups -= take;
                }
                answer = std::max(answer, current);
            }
        }
    }
    return answer;
}

}
