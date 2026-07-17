#include <cassert>
#include <map>
#include <random>
#include <vector>

#include "algorithm/rangefrequency.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int size = 0; size <= 60; ++size) {
        std::vector<int> values(size);
        for (auto& value : values) value = random() % 10;
        std::vector<std::pair<int, int>> ranges;
        for (int query = 0; query < 100; ++query) {
            int left = random() % (size + 1);
            int right = random() % (size + 1);
            if (left > right) std::swap(left, right);
            ranges.emplace_back(left, right);
        }
        for (int tuple_size = 0; tuple_size <= 5; ++tuple_size) {
            std::vector<long long> expected;
            for (const auto& [left, right] : ranges) {
                std::map<int, int> frequency;
                for (int index = left; index < right; ++index) ++frequency[values[index]];
                long long answer = tuple_size == 0;
                if (tuple_size > 0) {
                    for (const auto& [value, count] : frequency) {
                        answer += poe::small_binomial(count, tuple_size);
                    }
                }
                expected.push_back(answer);
            }
            assert(poe::equal_tuple_range_counts(values, ranges, tuple_size) == expected);
        }
    }
}
