#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

#include "algorithm/isotonic.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = 1 + random() % 7;
        std::vector<long long> values(n);
        for (auto& value : values) value = static_cast<int>(random() % 7) - 3;

        const auto result = poe::right_transfer_isotonic(values);
        assert(std::is_sorted(result.values.begin(), result.values.end()));
        assert(std::accumulate(result.values.begin(), result.values.end(), 0LL) ==
               std::accumulate(values.begin(), values.end(), 0LL));
        long long original_prefix = 0;
        long long result_prefix = 0;
        for (int i = 0; i + 1 < n; ++i) {
            original_prefix += values[i];
            result_prefix += result.values[i];
            assert(result_prefix <= original_prefix);
        }

        long long brute = std::numeric_limits<long long>::max();
        std::vector<long long> candidate(n);
        const long long target_sum = std::accumulate(values.begin(), values.end(), 0LL);
        std::function<void(int, long long)> search = [&](int index, long long sum) {
            if (index == n) {
                if (sum != target_sum) return;
                long long left = 0;
                long long right = 0;
                long long cost = 0;
                for (int i = 0; i + 1 < n; ++i) {
                    left += values[i];
                    right += candidate[i];
                    if (right > left) return;
                    cost += left - right;
                }
                brute = std::min(brute, cost);
                return;
            }
            const int low = index == 0 ? -5 : static_cast<int>(candidate[index - 1]);
            for (int value = low; value <= 5; ++value) {
                candidate[index] = value;
                search(index + 1, sum + value);
            }
        };
        search(0, 0);
        assert(brute != std::numeric_limits<long long>::max());
        assert(result.operations == brute);
    }
}
