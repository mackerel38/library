#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "dp/knapsack.hpp"

long long brute01(
    const std::vector<long long>& weights,
    const std::vector<long long>& values,
    long long capacity
) {
    long long answer = 0;
    for (int mask = 0; mask < (1 << weights.size()); ++mask) {
        long long weight = 0, value = 0;
        for (int item = 0; item < static_cast<int>(weights.size()); ++item) {
            if (mask >> item & 1) {
                weight += weights[item];
                value += values[item];
            }
        }
        if (weight <= capacity) answer = std::max(answer, value);
    }
    return answer;
}

long long brute_bounded(
    const std::vector<int>& weights,
    const std::vector<long long>& values,
    const std::vector<long long>& counts,
    int capacity,
    int item = 0
) {
    if (item == static_cast<int>(weights.size())) return 0;
    long long answer = 0;
    for (int count = 0; count <= counts[item] && count * weights[item] <= capacity; ++count) {
        answer = std::max(
            answer,
            count * values[item] + brute_bounded(
                weights, values, counts, capacity - count * weights[item], item + 1
            )
        );
    }
    return answer;
}

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 500; ++trial) {
        const int n = random() % 16;
        const int capacity = random() % 50;
        std::vector<long long> weights(n), values(n);
        std::vector<int> int_weights(n), int_values(n);
        for (int item = 0; item < n; ++item) {
            int_weights[item] = weights[item] = 1 + random() % 15;
            int_values[item] = values[item] = random() % 30;
        }
        const long long answer = brute01(weights, values, capacity);
        assert(poe::knapsack01(int_weights, values, capacity) == answer);
        assert(poe::knapsack01_value(weights, int_values, capacity) == answer);
        assert(poe::knapsack01_mitm(weights, values, capacity) == answer);
        assert(poe::knapsack01_auto(weights, values, capacity) == answer);
    }
    for (int trial = 0; trial < 300; ++trial) {
        const int n = random() % 7;
        const int capacity = random() % 35;
        std::vector<int> weights(n);
        std::vector<long long> values(n), counts(n);
        for (int item = 0; item < n; ++item) {
            weights[item] = 1 + random() % 10;
            values[item] = random() % 20;
            counts[item] = random() % 6;
        }
        assert(
            poe::bounded_knapsack(weights, values, counts, capacity)
            == brute_bounded(weights, values, counts, capacity)
        );
    }
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = 1 + random() % 7;
        const int target = 1 + random() % 120;
        std::vector<int> costs(n);
        std::vector<long long> values(n);
        for (int item = 0; item < n; ++item) {
            costs[item] = 1 + random() % 7;
            values[item] = 1 + random() % 12;
        }
        const int upper = target * costs[0];
        std::vector<long long> maximum_value(upper + 1, -1);
        maximum_value[0] = 0;
        int expected = upper;
        for (int cost = 1; cost <= upper; ++cost) {
            for (int item = 0; item < n; ++item) {
                if (costs[item] <= cost && maximum_value[cost - costs[item]] >= 0) {
                    maximum_value[cost] = std::max(
                        maximum_value[cost],
                        maximum_value[cost - costs[item]] + values[item]
                    );
                }
            }
            if (maximum_value[cost] >= target) {
                expected = cost;
                break;
            }
        }
        assert(poe::minimum_unbounded_knapsack_cost(costs, values, target) == expected);
    }
    for (int trial = 0; trial < 400; ++trial) {
        const int n = 1 + random() % 8;
        const int capacity = random() % 12;
        std::vector<int> weights(n);
        std::vector<long long> values(n);
        for (int item = 0; item < n; ++item) {
            weights[item] = random() % 7;
            values[item] = random() % 12;
        }
        long long optimum = -1;
        std::vector<int> optimal_masks;
        for (int mask = 0; mask < (1 << n); ++mask) {
            int weight = 0;
            long long value = 0;
            for (int item = 0; item < n; ++item) if (mask >> item & 1) {
                weight += weights[item];
                value += values[item];
            }
            if (weight > capacity) continue;
            if (value > optimum) optimum = value, optimal_masks.clear();
            if (value == optimum) optimal_masks.push_back(mask);
        }
        const auto status = poe::classify_knapsack01_items(weights, values, capacity);
        for (int item = 0; item < n; ++item) {
            int used = 0;
            for (int mask : optimal_masks) used += mask >> item & 1;
            const auto expected = used == 0 ? poe::knapsackitemstatus::excluded
                : used == static_cast<int>(optimal_masks.size())
                    ? poe::knapsackitemstatus::mandatory
                    : poe::knapsackitemstatus::optional;
            assert(status[item] == expected);
        }
    }
}
