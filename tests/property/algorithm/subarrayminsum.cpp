#include "algorithm/subarrayminsum.hpp"

int main() {
    std::mt19937_64 random(20260718);
    for (int trial = 0; trial < 20000; ++trial) {
        const int n = 1 + random() % 30;
        std::vector<long long> minimums(n), weights(n);
        for (auto& value : minimums) value = random() % 20;
        for (auto& value : weights) value = random() % 8;
        const long long limit = static_cast<long long>(random() % 100) - 10;
        long long expected = 0;
        for (int left = 0; left < n; ++left) {
            long long minimum = minimums[left];
            long long sum = 0;
            for (int right = left; right < n; ++right) {
                minimum = std::min(minimum, minimums[right]);
                sum += weights[right];
                expected += minimum + sum <= limit;
            }
        }
        assert(poe::count_subarrays_min_plus_sum_at_most(
                   minimums, weights, limit) == expected);
    }
}
