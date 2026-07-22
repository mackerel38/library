#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <vector>
#include "algorithm/subarrayrange.hpp"

int main() {
    std::mt19937 random(20260722);
    for (int size = 1; size <= 11; ++size) {
        std::vector<int> values(size);
        std::iota(values.begin(), values.end(), 1);
        for (int trial = 0; trial < 1000; ++trial) {
            std::shuffle(values.begin(), values.end(), random);
            for (int k = 0; k <= 4; ++k) {
                long long expected = 0;
                for (int left = 0; left < size; ++left) {
                    int minimum = values[left], maximum = values[left];
                    for (int right = left; right < size; ++right) {
                        minimum = std::min(minimum, values[right]);
                        maximum = std::max(maximum, values[right]);
                        expected += maximum - minimum - (right - left) <= k;
                    }
                }
                assert(poe::count_subarrays_range_excess_at_most(values, k) == expected);
            }
        }
    }
}
