#include <algorithm>
#include <cassert>
#include <random>
#include <vector>
#include "dp/knapsack.hpp"

using namespace poe;

int main() {
    std::mt19937 random(442);
    for (int trial = 0; trial < 1000; ++trial) {
        const int types = 1 + random() % 7;
        const int capacity = random() % 20;
        std::vector<int> weight(types);
        std::vector<long long> value(types), count(types);
        std::vector<std::pair<int, long long>> expanded;
        for (int item = 0; item < types; ++item) {
            weight[item] = 1 + random() % 3;
            value[item] = random() % 20;
            count[item] = random() % 4;
            for (int copy = 0; copy < count[item]; ++copy) {
                expanded.emplace_back(weight[item], value[item]);
            }
        }
        std::vector<long long> dp(capacity + 1);
        for (const auto [w, v] : expanded) {
            for (int total = capacity; total >= w; --total) {
                dp[total] = std::max(dp[total], dp[total - w] + v);
            }
        }
        assert(bounded_knapsack_tiny_weights(weight, value, count, capacity) == dp[capacity]);
    }
}
