#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "dp/segments.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = 1 + random() % 12;
        const int k = random() % 5;
        std::vector<long long> values(n);
        for (auto& value : values) value = static_cast<int>(random() % 15) - 7;
        long long brute = 0;
        for (int mask = 0; mask < (1 << n); ++mask) {
            int segments = 0;
            long long sum = 0;
            for (int i = 0; i < n; ++i) {
                if (mask >> i & 1) {
                    sum += values[i];
                    if (i == 0 || !(mask >> (i - 1) & 1)) ++segments;
                }
            }
            if (segments <= k) brute = std::max(brute, sum);
        }
        assert(poe::maximum_k_disjoint_subarray_sum(values, k) == brute);
    }
}
